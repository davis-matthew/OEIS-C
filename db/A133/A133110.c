/*
 * A133110  Triangle read by rows: A007318 * A133109.
 * https://oeis.org/A133110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r-1, k) * t(k) * where * (r-1)r/2 < n ≤ r(r+1)/2
 *            – r is the row number (1‑based) * k = n - ((r-1)r/2) - 1 –
 *            column index (0‑based) * t(k) = the k‑th positive integer NOT ≡
 *            2 (mod 4) * (t(k) = 4*⌊k/3⌋+1 if k%3==0, * 4*⌊k/3⌋+3 if k%3==1,
 *            * 4*⌊k/3⌋+4 if k%3==2) * * The triangle in OE...
 * Range:     u8 n <= 39, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; behaviour for n==0 is irrelevant */
        return 0;

    /* --- find r: minimal integer with r(r+1)/2 ≥ n --- */
    idx_t lo = 1, hi = 1;
    while (((unsigned __int128)hi * (hi + 1) / 2) < n)
        hi <<= 1;                      /* hi grows until the cumulative size covers n */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 midsum = (unsigned __int128)mid * (mid + 1) / 2;
        if (midsum < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo;                       /* row number (1‑based) */

    /* --- compute column index k (0‑based) --- */
    unsigned __int128 prefix = (unsigned __int128)(r - 1) * r / 2;   /* number of terms before row r */
    idx_t start = (idx_t)prefix + 1;                               /* first index in row r */
    idx_t k = n - start;                                          /* column within the row */

    /* --- binomial coefficient from A007318 (Pascal) --- */
    seq_t bin = binom(r - 1, k);

    /* --- compute t(k) (A133109) --- */
    idx_t block = k / 3;
    idx_t rem   = k % 3;
    seq_t t;
    if (rem == 0)
        t = (seq_t)(4 * block + 1);
    else if (rem == 1)
        t = (seq_t)(4 * block + 3);
    else
        t = (seq_t)(4 * block + 4);      /* rem == 2 */

    return bin * t;
}
