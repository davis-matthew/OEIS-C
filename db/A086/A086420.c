/*
 * A086420  Euler's totient of 3-smooth numbers: a(n) = A000010(A003586(n)).
 * https://oeis.org/A086420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 105, u32 n <= 370, u64 n <= 1385,
 *            u128 n <= 5354, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A086420 : Euler's totient of 3‑smooth numbers
 *
 * a(n) = φ( A003586(n) )
 * where A003586(n) is the n‑th number with no prime factors larger than 3,
 * i.e. numbers of the form 2^i·3^j (i,j≥0).  The sequence is generated
 * by keeping the pairs (i,j) in increasing order of value.
 *
 * This implementation builds the sequence incrementally and memoises the
 * results, so each call to a(n) is O(1) after the necessary terms have
 * been generated.  No hard‑coded table is used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ----------  Cached state  --------------------------------- */

static idx_t   len      = 0;      /* number of terms cached */
static idx_t   cap      = 0;      /* current array capacity          */
static idx_t   i2       = 0;      /* pointer for generating *2       */
static idx_t   i3       = 0;      /* pointer for generating *3       */

/* arrays holding the exponents of 2 and 3 for each generated number */
static idx_t *ex2     = NULL;     /* exponent of 2 for term k */
static idx_t *ex3     = NULL;     /* exponent of 3 for term k */

/* phi values of each term, stored as seq_t */
static seq_t *phiCache = NULL;

/* log2(3) once, used for ordering */
static const double LOG3 = 1.58496250072115618144;

/* ----------  Utility functions  ------------------------------- */

/* Compute φ(2^e2·3^e3) using the formula:
 *   φ(2^e2) = 2^(e2-1)   (if e2>0, else 1)
 *   φ(3^e3) = 2·3^(e3-1) (if e3>0, else 1)
 */
static seq_t compute_phi(idx_t e2, idx_t e3)
{
    seq_t result = (seq_t)1;

    /* contribution from the 2‑power */
    if (e2 > 0) {
        seq_t t = (seq_t)1;
        for (idx_t k = 0; k < e2 - 1; ++k)
            t = t * (seq_t)2;
        result = result * t;
    }

    /* contribution from the 3‑power */
    if (e3 > 0) {
        seq_t t = (seq_t)2;            /* φ(3) = 2 */
        for (idx_t k = 0; k < e3 - 1; ++k)
            t = t * (seq_t)3;
        result = result * t;
    }

    return result;
}

/* Grow the cached arrays to at least 'new_cap' elements. */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cap)
        return;

    idx_t old_cap = cap;
    if (old_cap == 0) old_cap = 4;
    while (old_cap < new_cap) old_cap <<= 1;
    cap = old_cap;

    ex2      = (idx_t *)realloc(ex2,      cap * sizeof(idx_t));
    ex3      = (idx_t *)realloc(ex3,      cap * sizeof(idx_t));
    phiCache = (seq_t *)realloc(phiCache, cap * sizeof(seq_t));
}

/* Extend the cache so that it contains at least 'target' terms. */
static void extend_to(idx_t target)
{
    if (len == 0) {          /* first initialisation */
        grow_cache(target);
        ex2[0] = (idx_t)0;
        ex3[0] = (idx_t)0;
        phiCache[0] = compute_phi(0, 0);   /* φ(1) = 1 */
        len = 1;
    }
    else if (cap < target) {
        grow_cache(target);
    }

    while (len < target) {
        /* Current candidates for the next term */
        double val2 = (double)(ex2[i2] + 1) + (double)ex3[i2] * LOG3;
        double val3 = (double)ex2[i3] + (double)(ex3[i3] + 1) * LOG3;

        if (val2 <= val3) {
            /* Take the '*'2' candidate */
            ex2[len] = ex2[i2] + 1;
            ex3[len] = ex3[i2];
            phiCache[len] = compute_phi(ex2[len], ex3[len]);

            /* Record whether both candidates are equal */
            int equal = ((ex2[i2] + 1 == ex2[i3]) && (ex3[i2] == ex3[i3] + 1));
            ++i2;
            if (equal) ++i3;
        }
        else {
            /* Take the '*'3' candidate */
            ex2[len] = ex2[i3];
            ex3[len] = ex3[i3] + 1;
            phiCache[len] = compute_phi(ex2[len], ex3[len]);

            ++i3;
        }

        ++len;
    }
}

/* ----------  Function requested by the harness  ---------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of OEIS, harmless */
    extend_to(n);                         /* generate up to n */
    return phiCache[n - 1];                /* 1‑based indexing */
}
