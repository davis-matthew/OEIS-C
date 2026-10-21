/*
 * A211805  Rectangular array: R(k,n) = number of ordered triples (w,x,y) with
 *          all terms in {1,...,n} and 2w^k>=x^k+y<k.
 * https://oeis.org/A211805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of triples (w,x,y) in {1,…,m} with 2*w^k ≥ x^k+y^k
 *            for the pair (k,m) that corresponds to the nth entry in the
 *            antidiagonal enumeration of the rectangular array R(k,m). The
 *            conversion from a sequence index n to (k,m) is: let s be the
 *            unique integer such that (s-1)*s/2 ≥ n > (s-2)*(s...
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note:  R(k,m) is computed lazily and cached.  */

#include <stdlib.h>
#include "../utils/big_pow.h"

#define MAXK 400   /* sufficient for n up to ≈10 000 (s≈142) */

/* Cached values for R(k,m) */
static seq_t R_cache[MAXK+1][MAXK+1];
static unsigned char R_done[MAXK+1][MAXK+1];

/* Forward declaration */
static seq_t compute_R(idx_t k, idx_t m);

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Find the diagonal level s */
    idx_t s = 2;
    while ( (s-1)*s/2 < n ) ++s;

    /* Compute prev total terms before this diagonal */
    idx_t prev = (s-2)*(s-1)/2;

    /* Offset within the diagonal */
    idx_t r = n - prev - 1;

    /* Convert to (k, m) */
    idx_t k = r + 1;
    idx_t m = s - 1 - r;

    /* Ensure parameters are within bounds */
    if (k > MAXK || m > MAXK) {
        /* For safety: recompute with larger arrays if needed.
           In practice, for the typical n range this never happens. */
        return 0;
    }

    return compute_R(k, m);
}

/* Compute R(k,m): number of triples (w,x,y) with 1≤w,x,y≤m
   satisfying 2*w^k ≥ x^k + y^k.  */
static seq_t compute_R(idx_t k, idx_t m)
{
    if (R_done[k][m]) return R_cache[k][m];

    /* Precompute i^k for i=1..m */
    seq_t pow_k[MAXK+1];
    pow_k[0] = 0;               /* unused */
    for (idx_t i = 1; i <= m; ++i)
        pow_k[i] = big_pow((seq_t)i, k);

    seq_t total = 0;
    const seq_t two = 2;

    for (idx_t w = 1; w <= m; ++w) {
        seq_t threshold = pow_k[w] * two;

        /* Count ordered pairs (x,y) with x^k + y^k ≤ threshold */
        idx_t i = 1;
        idx_t j = m;
        seq_t count = 0;

        while (i <= m) {
            while (j >= 1 && (pow_k[i] + pow_k[j]) > threshold) --j;
            if (j < 1) break;
            count += (seq_t)j;  /* add all y from 1..j for this x */
            ++i;
        }

        total += count;
    }

    R_cache[k][m] = total;
    R_done[k][m] = 1;
    return total;
}
