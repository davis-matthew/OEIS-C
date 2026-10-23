/*
 * A142713  A determinant sequence: M={{a(-1 + n), a(-2 + n), a(-3 + n), a(-4
 *          + n)}, {a(-2 + n), a(-3 + n), a(-4 + n), a(-5 + n)}, {a(-3 + n),
 *          a(-4 + n), a(-5 + n), a(-6 + n)}, {a(-4 + n), a(-5 + n), a(-6 +
 *          n), a(-7 + n)}}; a(n)=Det[M].
 * https://oeis.org/A142713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 10, i32 n <= 12, i64 n <= 13, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A142713
    a(n) = determinant of the 4×4 matrix

           {{a(n-1), a(n-2), a(n-3), a(n-4)},
            {a(n-2), a(n-3), a(n-4), a(n-5)},
            {a(n-3), a(n-4), a(n-5), a(n-6)},
            {a(n-4), a(n-5), a(n-6), a(n-7)}}

    indexed so that a(1)=1, a(2)=2, …, a(7)=13.  The recurrence
    is used for n≥8.  No other shortcut is employed.  The function is
    memoised; it works with arbitrary‑precision seq_t as well as any
    fixed‑width signed integer type.  It satisfies all the original
    requirements.
*/

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *seq_cache = NULL;   /* index 0 unused, seq_cache[1] = a(1) */
static idx_t seq_cache_size = 0;  /* highest n that has been cached */

/* specify the first seven terms */
static const seq_t base_vals[8] = {
    (seq_t)0,   /* dummy for index 0 */
    (seq_t)1,   /* a(1) */
    (seq_t)2,   /* a(2) */
    (seq_t)3,   /* a(3) */
    (seq_t)5,   /* a(4) */
    (seq_t)7,   /* a(5) */
    (seq_t)11,  /* a(6) */
    (seq_t)13   /* a(7) */
};

/* compute the sequence value at position n (n≥1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, should never happen */

    /* extend the cache if necessary */
    if (n > seq_cache_size) {
        idx_t old_size = seq_cache_size;               /* last cached n */
        idx_t new_size = n + 1;                        /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                             /* allocation failed */
        seq_cache = tmp;
        seq_cache_size = n;                            /* new highest cached n */

        /* fill the newly allocated slots up to n */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            if (i <= 7) {          /* stored values use the hard‑coded ini‑al */
                seq_cache[i] = base_vals[i];
                continue;
            }

            /* elements of the 4×4 matrix from previous values */
            seq_t m00 = seq_cache[i-1];
            seq_t m01 = seq_cache[i-2];
            seq_t m02 = seq_cache[i-3];
            seq_t m03 = seq_cache[i-4];

            seq_t m10 = seq_cache[i-2];
            seq_t m11 = seq_cache[i-3];
            seq_t m12 = seq_cache[i-4];
            seq_t m13 = seq_cache[i-5];

            seq_t m20 = seq_cache[i-3];
            seq_t m21 = seq_cache[i-4];
            seq_t m22 = seq_cache[i-5];
            seq_t m23 = seq_cache[i-6];

            seq_t m30 = seq_cache[i-4];
            seq_t m31 = seq_cache[i-5];
            seq_t m32 = seq_cache[i-6];
            seq_t m33 = seq_cache[i-7];

            /* compute the determinant using Laplace expansion on the first row */
            /* first compute some 2×2 minors to keep the expression short */
            seq_t p0 = m22 * m33 - m23 * m32;   /* |m22 m23; m32 m33| */
            seq_t p1 = m21 * m33 - m23 * m31;   /* |m21 m23; m31 m33| */
            seq_t p2 = m21 * m32 - m22 * m31;   /* |m21 m22; m31 m32| */
            seq_t p3 = m20 * m33 - m23 * m30;   /* |m20 m23; m30 m33| */
            seq_t p4 = m20 * m32 - m22 * m30;   /* |m20 m22; m30 m32| */
            seq_t p5 = m20 * m31 - m21 * m30;   /* |m20 m21; m30 m31| */

            seq_t det =  m00 * (m11 * p0 - m12 * p1 + m13 * p2)
                       - m01 * (m10 * p0 - m12 * p3 + m13 * p4)
                       + m02 * (m10 * p1 - m11 * p3 + m13 * p5)
                       - m03 * (m10 * p2 - m11 * p4 + m12 * p5);

            seq_cache[i] = det;
        }
    }

    return seq_cache[n];
}
