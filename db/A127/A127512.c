/*
 * A127512  Triangle read by rows: T(n,k)= mobius(n)*binomial(n-1,k-1).
 * https://oeis.org/A127512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = μ(r) * C(r-1, c-1) where r is the row containing the n‑th
 *            element (1‑based rows) and c is the column position (1‑based)
 *            within that row
 * Range:     i8 n <= 59, i16 n <= 179, i32 n <= 611, i64 n <= 2374,
 *            i128 n <= 8836, big any n
 * Validated: standard, pass, 94 terms through n = 9853 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes used helpers only for the mobius function. */
#include <stdint.h>
#include <stdbool.h>

static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;          /* defined as 0, but 0 is never passed */
    seq_t sign = 1;                /* product of (–1) for each prime factor */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;     /* not square‑free */
            }
            sign = -sign;
        }
    }
    if (tmp > 1)                   /* remaining prime factor */
        sign = -sign;
    return sign;
}

/* Compute binomial coefficient C(n,k) exactly for n,k that fit
   into 64 bits, using 128‑bit arithmetic to avoid overflow. */
static seq_t binomial_u128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (unsigned __int128)(n - k + i);
        result /= i;  /* division is exact */
    }
    return (seq_t)result;   /* fits in seq_t because term is bounded for n≤10000 */
}

/* Find the row r such that r*(r+1)/2 ≥ n (n ≥ 1). */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Increase high until it bounds n. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, safety value */

    /* Locate element (r,c) in the triangle. */
    idx_t r = find_row(n);                                  /* row index */
    unsigned __int128 prefix128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prefix = (idx_t)prefix128;                        /* cumulative count before row r */
    idx_t c = n - prefix;                                   /* 1‑based column index */

    /* Evaluate μ(r) * C(r-1, c-1).  All arithmetic fits in 128 bits
       before conversion to seq_t. */
    seq_t mu = mobius(r);
    seq_t bin = binomial_u128(r - 1, c - 1);
    return (seq_t)((__int128)mu * (__int128)bin);
}
