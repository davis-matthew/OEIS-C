/*
 * A116749  Number of permutations of length n which avoid the patterns 1423,
 *          3124, 3421.
 * https://oeis.org/A116749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A116749
 *  Formula: a(n) = 2^(n‑5)*(n+9)*(n‑2) -(n‑1)*(n^2+4*n-6)/6 + A001906(n),  n≥1
 *  The implementation uses the rational generating function
 *  G(x)=P(x)/Q(x) from the OEIS entry, which yields a linear
 *  recurrence of order 9.  The sequence is then computed incrementally
 *  and cached so that any call ordering works efficiently.
 */

#include <stdlib.h>
#include <string.h>

/* Pre‑computed coefficients p[n] for 0 ≤ n ≤ 10 of the numerator P(x).  */
static const int P[11] = {
    0,      /* p0  */
   -1,      /* p1  */
   11,      /* p2  */
  -53,      /* p3  */
  143,      /* p4  */
 -231,      /* p5  */
  230,      /* p6  */
 -153,      /* p7  */
   80,      /* p8  */
  -30,      /* p9  */
   5       /* p10 */
};

/* Cached values of the sequence.  */
static seq_t *seq_cache      = NULL;
static idx_t  seq_cache_size = 0;   /* allocated size (elements)     */
static idx_t  seq_cache_max  = 0;   /* maximum index computed         */

/* Ensure we have enough space to store indices up to `n`. */
static void ensure_capacity(idx_t n)
{
    if (seq_cache_size <= n) {
        idx_t new_size = n + 1;            /* indices 0 … n inclusive      */
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {                  /* allocation failed              */
            exit(1);
        }
        /* Copy any existing data to the new buffer. */
        if (seq_cache) {
            memcpy(new_cache, seq_cache, seq_cache_size * sizeof(seq_t));
            free(seq_cache);
        }
        seq_cache      = new_cache;
        seq_cache_size = new_size;
    }
}

/* Main routine.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1; a(0)=0 by the G.F. */

    /* Allocate cache if this is the first call.  */
    if (!seq_cache) {
        seq_cache_size = 0;
        seq_cache_max  = 0;
        ensure_capacity(n);    /* ensure we have room for n */
        seq_cache[0] = 0;      /* a(0) = 0  (G.F. has a factor of x) */
    }

    /* Expand the cache up to the requested index.  */
    if (seq_cache_max < n) {
        /* Make sure the buffer can hold indices up to n.  */
        ensure_capacity(n);

        for (idx_t k = seq_cache_max + 1; k <= n; ++k) {
            /* Perform the 9‑term recurrence derived from the G.F.:
             *
             * a(k) = 13*a(k-1) – 73*a(k-2) + 232*a(k-3) – 459*a(k-4)
             *       + 585*a(k-5) – 479*a(k-6) + 242*a(k-7) – 68*a(k-8)
             *       + 8*a(k-9) – p(k)
             *
             * The -p(k) term is handled by adding |p(k)| to the
             * positive side when p(k) < 0, and to the negative side when
             * p(k) > 0.
             */

            seq_t pos = 0, neg = 0;
            /* Positive contributions */
            pos += (seq_t)13  * seq_cache[k - 1];
            if (k >= 3) pos += (seq_t)232 * seq_cache[k - 3];
            if (k >= 5) pos += (seq_t)585 * seq_cache[k - 5];
            if (k >= 7) pos += (seq_t)242 * seq_cache[k - 7];
            if (k >= 9) pos += (seq_t)8   * seq_cache[k - 9];
            /* Handle the p(k) term */
            if (k <= 10) {
                int pk = P[k];
                if (pk < 0) pos += (seq_t)(-pk);   /* -p(k) adds |p(k)| */
            }

            /* Negative contributions */
            if (k >= 2) neg += (seq_t)73  * seq_cache[k - 2];
            if (k >= 4) neg += (seq_t)459 * seq_cache[k - 4];
            if (k >= 6) neg += (seq_t)479 * seq_cache[k - 6];
            if (k >= 8) neg += (seq_t)68  * seq_cache[k - 8];
            if (k <= 10) {
                int pk = P[k];
                if (pk > 0) neg += (seq_t)pk;       /* -p(k) subtracts p(k) */
            }

            /* The recurrence guarantees that pos ≥ neg. */
            seq_cache[k] = pos - neg;
        }

        seq_cache_max = n;
    }

    return seq_cache[n];
}
