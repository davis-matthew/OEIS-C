/*
 * A128636  Expansion of 3 * (b(q^2)^2 / b(q)) / (c(q)^2 / c(q^2)) in powers
 *          of q where b(), c() are cubic AGM theta functions.
 * https://oeis.org/A128636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 28, i32 n <= 85, i64 n <= 289,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A128636 – “Expansion of 3 * (b(q^2)^2 / b(q)) / (c(q)^2 / c(q^2)) in powers of q”.
 * The generating function is:
 *   G(x) = Prod_{k>0} (1 - x^(6k)) / (1 - x^k) * ((1 - x^(2k)) / (1 - x^(3k)))^5 .
 * The coefficients a(n) are obtained by sequentially multiplying the power series by
 * each factor (1 - x^m)^{e_m} with e_m:
 *     e_m = 0          if m ≡ 0 (mod 6)
 *     e_m = 4          if m ≡ 2,4 (mod 6)
 *     e_m = -6         if m ≡ 3 (mod 6)
 *     e_m = -1         otherwise
 * For e_m = 4 (positive) we use the polynomial expansion of (1 - z)^4.
 * For e_m = -1 we multiply by (1 - z)^{-1} via cumulative sums.
 * For e_m = -6 we apply the e_m = -1 operation six times.
 * The routine uses memoisation to avoid recomputation for successive calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* cache for computed sequence values */
static seq_t *cache = NULL;      /* pointer to array containing a(0 … N) */
static idx_t  cache_size = 0;    /* number of stored terms  (= N+1) */

/* the requested term a(n) */
seq_t a(idx_t n)
{
    /* return if already cached */
    if (n < cache_size) {
        return cache[n];
    }

    /* compute up to n */
    idx_t N = n;
    size_t Nl   = (size_t)N;
    idx_t  sz   = Nl + 1;
    /* allocate two working arrays of size N+1 */
    seq_t *arr   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *buffer = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!arr || !buffer) return (seq_t)0;  /* allocation failed */

    /* initial series A(x) = 1  (only a(0)=1) */
    arr[0] = (seq_t)1;

    seq_t *curr   = arr;   /* current series */
    seq_t *next   = buffer;  /* buffer for next series */

    /* process each factor (1 - x^m)^{e_m} for m = 1 .. N */
    for (idx_t m = 1; m <= N; ++m) {
        int mod = (int)(m % 6);
        int e   = 0;
        if (mod == 0)          e = 0;
        else if (mod == 2 || mod == 4) e = 4;
        else if (mod == 3)     e = -6;
        else                   e = -1;

        if (e == 0) continue;   /* skip trivial factor */

        size_t mlen = (size_t)m;

        if (e == 4) {
            /* (1 - z)^4 = 1 - 4z + 6z^2 - 4z^3 + z^4, z = x^m */
            for (size_t i = 0; i <= Nl; ++i) {
                seq_t val = curr[i];
                if (i >= mlen) val -= (seq_t)4 * curr[i - mlen];
                if (i >= mlen * 2) val += (seq_t)6 * curr[i - mlen * 2];
                if (i >= mlen * 3) val -= (seq_t)4 * curr[i - mlen * 3];
                if (i >= mlen * 4) val += curr[i - mlen * 4];
                next[i] = val;
            }
            /* swap */
            seq_t *tmp = curr; curr = next; next = tmp;
        }
        else if (e == -1) {
            /* multiply by (1 - z)^{-1}: cumulative sums */
            for (size_t i = 0; i <= Nl; ++i) {
                seq_t val = curr[i];
                if (i >= mlen) val += next[i - mlen];
                next[i] = val;
            }
            /* swap */
            seq_t *tmp = curr; curr = next; next = tmp;
        }
        else if (e == -6) {
            /* (1 - z)^{-6} = ((1 - z)^{-1})^6 */
            for (int step = 0; step < 6; ++step) {
                for (size_t i = 0; i <= Nl; ++i) {
                    seq_t val = curr[i];
                    if (i >= mlen) val += next[i - mlen];
                    next[i] = val;
                }
                /* swap */
                seq_t *tmp = curr; curr = next; next = tmp;
            }
        }
    }

    /* free obsolete buffer */
    seq_t *other = next;          /* the array that is not 'curr' */
    if (other != curr) free(other);

    /* update cache */
    free(cache);                   /* free previous cache if any */
    cache      = curr;             /* keep final series */
    cache_size = sz;               /* number of stored terms */

    return cache[n];
}
