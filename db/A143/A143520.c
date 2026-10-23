/*
 * A143520  a(n) is n times number of divisors of n if n is odd, zero if n is
 *          twice odd, n times number of divisors of n/4 if n is divisible by
 *          4.
 * https://oeis.org/A143520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * A112329(n) (n × number of divisors if n odd, 0 if n =
 *            2 × odd, n × (e-1) × A112329(odd_part) if n = 2^e × odd_part, e
 *            ≥ 2)
 * Range:     u8 n <= 44, u16 n <= 2879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Local helper: number of divisors of an odd integer. */
static seq_t divisor_count_odd(uint64_t n) {
    seq_t d = 1;
    uint64_t m = n;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)   /* remaining prime factor */
        d *= 2;
    return d;    /* d(1) = 1 is handled correctly (loop skipped) */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety, not used in OEIS offset 1 */

    /* Count the exponent e of 2 in n. */
    idx_t tmp = n;
    size_t e = 0;
    while ((tmp & 1ULL) == 0) {
        ++e;
        tmp >>= 1;
    }

    if (e == 1)        /* n = 2 × odd */
        return 0;

    if (e == 0) {      /* n is odd */
        seq_t d = divisor_count_odd((uint64_t)n);
        return (seq_t)n * d;
    }

    /* e >= 2 : n divisible by 4. tmp is the odd part. */
    seq_t d = divisor_count_odd(tmp);
    return (seq_t)n * (seq_t)(e - 1) * d;
}
