/*
 * A099089  Riordan array (1, 2+x).
 * https://oeis.org/A099089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the entry T(r,k) of the Riordan array (1,2+x) read
 *            row‑wise. For a given sequence index n we first determine the
 *            row r and column k such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k =
 *            n – r(r+1)/2. The element T(r,k) is 0 when k < r‑k. Otherwise
 *            T(r,k) = binomial(k, r‑k) · 2^{2k‑r} We compute t...
 * Range:     u8 n <= 42, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

static inline idx_t row_of(idx_t n)
{
    /* Return maximum r with r*(r+1)/2 ≤ n.  Binary search using 128‑bit maths. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from the linear index n. */
    idx_t r = row_of(n);
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute T(r,k).  Zero if k < r‑k (outside the triangle's support). */
    idx_t m = r - k;                     /* m = r – k = n – 2k */
    if (k < m) return 0;

    seq_t bin   = binom(k, m);           /* binomial(k, m) */
    seq_t pow   = pow2((idx_t)(2 * k - r));  /* 2^{2k‑r}          */
    return bin * pow;
}
