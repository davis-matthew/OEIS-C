/*
 * A073841  LCM of the composite numbers between n and 2n (both inclusive).
 * https://oeis.org/A073841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm{ m : n ≤ m ≤ 2n, m composite } \n Implements the OEIS
 *            sequence A073841
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 18, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/*  a(n) returns the least common multiple of all composite integers
 *  between n and 2n (inclusive).  0 is treated as a nonsense index
 *  and returns 1.  The function works for any build of seq_t and
 *  idx_t (uint64_t) and is O(n) per call with a small sieve.  */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Interval [n,2n] */
    idx_t max = n * 2;

    /* 12‑bit sieve array: 1 → prime, 0 → composite / non‑prime */
    size_t sz = (size_t)max + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) return 0;          /* allocation failed */

    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    for (idx_t p = 2; p * p <= max; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= max; m += p) is_prime[m] = 0;
    }

    seq_t result = 1;

    /* For each prime p ≤ n find the highest power p^e that
     * divides some composite number in [n,2n].  */
    for (idx_t p = 2; p <= max; ++p) {
        if (p > n) break;              /* primes > n cannot appear */
        if (!is_prime[p]) continue;    /* skip non‑primes */

        __uint128_t p_pow = p;         /* current power p^k */
        idx_t e = 0;
        while (1) {
            /* Existence test of a multiple of p_pow inside [n,2n] */
            __uint128_t ceil_n  = ((__uint128_t)n + p_pow - 1) / p_pow;
            __uint128_t floor_2n = (__uint128_t)max / p_pow;
            if (ceil_n > floor_2n) break;   /* no such multiple */

            ++e;   /* we have at least one composite divisible by p_pow */

            /* Avoid overflow when computing next power */
            if (p_pow > ((__uint128_t)max) / p) break;
            p_pow *= p;
        }

        if (e > 0) {
            seq_t p_pow_seq = 1;
            for (idx_t i = 0; i < e; ++i)
                p_pow_seq *= (seq_t)p;
            result *= p_pow_seq;
        }
    }

    free(is_prime);
    return result;
}
