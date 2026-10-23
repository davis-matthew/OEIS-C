/*
 * A123516  Triangle read by rows: T(n,k) = (-1)^k * n! * 2^(n-2*k) *
 *          binomial(n,k) * binomial(2*k,k) (0<=k<=n).
 * https://oeis.org/A123516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   let r be the largest integer with r*(r+1)/2 ≤ n, k = n –
 *            r*(r+1)/2. Then a(n) = (-1)^k * r! * 2^(r-2k) * C(r,k) * C(2k,k)
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 46, i64 n <= 123, i128 n <= 333,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Find the largest r with r*(r+1)/2 ≤ n using binary search. */
static idx_t row_from_n(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n)
        high <<= 1;                /* exponential growth, safe for n ≤ 2^63-1 */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* OEIS sequence A123516. */
seq_t a(idx_t n)
{
    idx_t r = row_from_n(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t t = factorial(r);                /* r! */
    t *= binomial(r, k);                   /* * C(r,k) */
    t *= binomial(2 * k, k);               /* * C(2k,k) */

    idx_t exp = (r >= 2 * k) ? (r - 2 * k) : (2 * k - r);
    if (r >= 2 * k)
        t *= pow2(exp);                    /* * 2^(r-2k) */
    else
        t /= pow2(exp);                    /* / 2^(2k-r), exact division */

    if (k & 1U)                            /* (-1)^k factor */
        t = -t;

    return t;
}
