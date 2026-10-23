/*
 * A189883  Numbers k such that the square part of k is one greater than the
 *          squarefree part of k.
 * https://oeis.org/A189883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x^2 * (x^2 - 1) where x is the nth positive integer for
 *            which x^2 - 1 is squarefree (OEIS A067874)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values, safe from overflow */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Estimate an upper bound for x that should contain at least n values. */
    for (idx_t bound = n * 5 + 100; ; bound *= 2) {
        idx_t sqrtmax = int_sqrt_u64(bound + 1);

        /* Allocate validation array: 1 means possibly squarefree, 0 means not. */
        unsigned char *valid = (unsigned char*)malloc((size_t)(bound + 1));
        if (!valid)
            return (seq_t)0;   /* allocation failure */
        for (size_t i = 0; i <= (size_t)bound; ++i)
            valid[i] = (unsigned char)1;
        if (bound >= 0) valid[0] = 0;
        if (bound >= 1) valid[1] = 0;   /* x must be at least 2 */

        /* Sieve primes up to sqrtmax. */
        bool *is_prime = (bool*)malloc((size_t)(sqrtmax + 1));
        if (!is_prime) {
            free(valid);
            return (seq_t)0;
        }
        for (size_t i = 0; i <= (size_t)sqrtmax; ++i)
            is_prime[i] = true;
        if (sqrtmax >= 0) is_prime[0] = false;
        if (sqrtmax >= 1) is_prime[1] = false;
        for (idx_t p = 2; p * p <= sqrtmax; ++p) {
            if (!is_prime[p])
                continue;
            for (idx_t q = p * p; q <= sqrtmax; q += p)
                is_prime[q] = false;
        }

        /* Mark x that are not squarefree because (x±1) is divisible by a square > 1. */
        for (idx_t p = 2; p <= sqrtmax; ++p) {
            if (!is_prime[p])
                continue;
            idx_t p2 = p * p;
            /* Mark all x ≡ 1 (mod p2) */
            idx_t start = 1;
            if (start < 2)
                start += ((2 - start + p2 - 1) / p2) * p2;
            for (idx_t x = start; x <= bound; x += p2)
                valid[x] = 0;
            /* Mark all x ≡ -1 (mod p2)  →  x = p2 - 1 (mod p2) */
            start = p2 - 1;
            if (start < 2)
                start += ((2 - start + p2 - 1) / p2) * p2;
            for (idx_t x = start; x <= bound; x += p2)
                valid[x] = 0;
        }

        /* Find the n‑th valid x */
        idx_t count = 0;
        idx_t x_found = 0;
        for (idx_t x = 2; x <= bound; ++x) {
            if (valid[x]) {
                ++count;
                if (count == n) {
                    x_found = x;
                    break;
                }
            }
        }

        free(valid);
        free(is_prime);

        if (x_found != 0) {
            seq_t xx = (seq_t)x_found * (seq_t)x_found;   /* x^2 */
            return xx * (xx - 1);                         /* x^2 * (x^2 - 1) */
        }
        /* Not enough values found; increase bound and repeat. */
    }
}
