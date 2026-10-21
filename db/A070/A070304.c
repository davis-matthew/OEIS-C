/*
 * A070304  a(n) = number of times n^2*k^2/(n^2+k^2) is an integer as k ranges
 *          over 0, 1, 2, ...
 * https://oeis.org/A070304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the sequence A070304.  
   The definition: a(n) = number of k >= 0 such that
                     n^2 * k^2 / (n^2 + k^2) is an integer.  */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Euclidean gcd for unsigned 64‑bit integers. */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1 */

    seq_t count = 0;                /* total number of valid k */

    /* iterate over all divisors g of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* first divisor g = i */
        {
            idx_t g = i;
            idx_t n1 = n / g;                /* n1 = n/g  */
            if (g >= n1) {                  /* g^2 >= n1^2 ensures existence of k1 */
                uint64_t diff_sq = (uint64_t)g * g - (uint64_t)n1 * n1;
                idx_t max_k1 = (idx_t)floor(sqrt((long double)diff_sq));
                idx_t min_k1 = (g == n) ? 0 : 1;  /* k1=0 only valid when g=n (k=0) */
                for (idx_t k1 = min_k1; k1 <= max_k1; ++k1) {
                    if (k1 == 0 && g != n) continue;   /* skip k1=0 for g != n */
                    if (k1 != 0 && gcd_uint64(k1, n1) != 1) continue;
                    uint64_t D = (uint64_t)n1 * n1 + (uint64_t)k1 * k1;
                    if ((uint64_t)g * g % D == 0) {
                        count++;
                    }
                }
            }
        }

        /* second divisor g = n / i, avoid duplicate when i*i == n */
        if (i * i == n) continue;          /* i^2 == n, both divisors equal, already processed */
        {
            idx_t g = n / i;
            idx_t n1 = n / g;
            if (g >= n1) {
                uint64_t diff_sq = (uint64_t)g * g - (uint64_t)n1 * n1;
                idx_t max_k1 = (idx_t)floor(sqrt((long double)diff_sq));
                idx_t min_k1 = (g == n) ? 0 : 1;
                for (idx_t k1 = min_k1; k1 <= max_k1; ++k1) {
                    if (k1 == 0 && g != n) continue;
                    if (k1 != 0 && gcd_uint64(k1, n1) != 1) continue;
                    uint64_t D = (uint64_t)n1 * n1 + (uint64_t)k1 * k1;
                    if ((uint64_t)g * g % D == 0) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}
