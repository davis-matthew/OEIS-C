/*
 * A072273  Index of powers of 2 that equal the number of noncongruent roots
 *          to the congruence x^2 == k (mod n) for (k,n)=1 and assuming
 *          solvability.
 * https://oeis.org/A072273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω_odd(n) + i, where * ω_odd(n) = number of distinct odd
 *            prime factors of n, * i = 0 if v2(n) ≤ 1, i = 1 if v2(n) = 2, *
 *            i = 2 if v2(n) ≥ 3. * This yields 2^a(n) = A060594(n)
 * Range:     u8 n <= 9776, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Count v2(n) */
    uint64_t j = 0;
    uint64_t tmp = (uint64_t)n;
    while ((tmp & 1ULL) == 0) {
        j++;
        tmp >>= 1;
    }

    /* Count distinct odd prime factors of the odd part */
    uint64_t odd = tmp;  /* Now odd part of n */
    uint64_t cnt = 0;
    for (uint64_t p = 3; p * p <= odd; p += 2) {
        if (odd % p == 0) {
            cnt++;
            while (odd % p == 0) odd /= p;
        }
    }
    if (odd > 1ULL) cnt++;  /* Remaining prime > sqrt */

    /* Compute the i term */
    uint64_t i;
    if (j <= 1ULL)          i = 0;
    else if (j == 2ULL)     i = 1;
    else                    i = 2;

    return (seq_t)(cnt + i);
}
