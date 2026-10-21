/*
 * A086418  Sum of distinct prime factors of 3-smooth numbers.
 * https://oeis.org/A086418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (i > 0 ? 2 : 0) + (j > 0 ? 3 : 0) where 2^i 3^j is the
 *            n‑th 3‑smooth number (1 ≤ n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the 3‑smooth numbers in ascending order using the
   standard two‑pointer method.  The actual values are stored as `long double`
   so that working range comfortably exceeds any number that can occur for
   the test limits (≈10^23 for n≈10 000).  Only the exponents are needed
   to compute the term value.  */
#include <stdint.h>
#include <stdlib.h>

/* Exponents of 2 and 3 for each 3‑smooth number. */
typedef struct { unsigned i, j; } Exp;

/* static cache of exponents and values */
static Exp         *exps      = NULL;   /* exponent pairs   */
static long double *valsL     = NULL;   /* numeric values   */
static size_t       len       = 0;      /* number of terms    */
static size_t       cap       = 0;      /* allocated space   */
static size_t       ptr2      = 0;      /* next multiple of 2 */
static size_t       ptr3      = 0;      /* next multiple of 3 */

/* Grow the arrays until they can hold at least `min_cap` elements. */
static void grow_cap(size_t min_cap)
{
    if (cap >= min_cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;

    Exp *tmpE  = (Exp *)realloc(exps,  new_cap * sizeof( Exp ));
    long double *tmpV = (long double *)realloc(valsL, new_cap * sizeof( long double ));
    if (!tmpE || !tmpV) abort();   /* allocation failure */

    exps  = tmpE;
    valsL = tmpV;
    cap   = new_cap;
}

/* Ensure that at least `n` 3‑smooth numbers are generated.  */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (len >= need) return;          /* already available */

    if (len == 0) {          /* initialise first term */
        grow_cap(16);
        exps[0].i = 0; exps[0].j = 0;
        valsL[0] = 1.0L;
        len = 1;
        ptr2 = ptr3 = 0;
    }

    while (len < need) {
        grow_cap(len + 1);

        long double v2 = valsL[ptr2] * 2.0L;
        long double v3 = valsL[ptr3] * 3.0L;

        if (v2 < v3) {
            /* use the next multiple of 2 */
            valsL[len] = v2;
            exps[len].i = exps[ptr2].i + 1;
            exps[len].j = exps[ptr2].j;
            ptr2++;
        } else if (v3 < v2) {
            /* use the next multiple of 3 */
            valsL[len] = v3;
            exps[len].i = exps[ptr3].i;
            exps[len].j = exps[ptr3].j + 1;
            ptr3++;
        } else {
            /* equality cannot happen for 3‑smooth numbers, but keep logic
               for completeness */
            valsL[len] = v2;
            exps[len].i = exps[ptr2].i + 1;
            exps[len].j = exps[ptr2].j;
            ptr2++;
            ptr3++;
        }
        len++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0; /* undefined for n<1, but return 0 */

    ensure_cache(n);
    size_t idx  = (size_t)(n - 1);   /* 0‑based index */
    unsigned i = exps[idx].i;
    unsigned j = exps[idx].j;

    seq_t val = 0;
    if (i) val += 2;
    if (j) val += 3;
    return val;
}
