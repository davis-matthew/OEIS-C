/*
 * A118832  Denominators of the convergents of the 2-adic continued fraction
 *          of zero given by A118830.
 * https://oeis.org/A118832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   tz_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118832 – Denominators of the convergents of the 2‑adic continued fraction of zero (A118830).
   Formula (OEIS notation):
       a(4*n)   = (-1)^n * 2 * A080277(n)
       a(4*n+1) = -(-1)^n * (2*A080277(n)-1)
       a(4*n+2) = -(-1)^n * (2*A080277(n)-2)
       a(4*n-1) = (-1)^n
   The auxiliary sequence A080277 satisfies:
       A080277(0) = 0
       A080277(1) = 1
       For n > 1:
           if n is odd:  A080277(n) = A080277(n-1) + 1
           if n is even: A080277(n) = A080277(n-1) + (2^(tz(n/2)+2) - 1)
   (tz(m) = number of trailing zero bits of m).
*/

#include <stdlib.h>
#include "../utils/tz_count.h"

/*--- auxiliary -------------------------------------------------------*/

static seq_t *A = NULL;          /* array storing A080277[0..] */
static idx_t A_size = 0;         /* number of allocated slots (A[0] exists when A_size>=1) */

/* Ensure that A[0..k] are computed. */
static void ensure_A(idx_t k)
{
    if (k == 0) {
        /* Need at least A[0] */
        if (!A) {
            A = (seq_t *)malloc(sizeof(seq_t));
            if (!A) return;            /* allocation failed – leave it undefined */
            A[0] = (seq_t)0;
            A_size = 1;
        }
        return;
    }

    /* First allocation for A[0] and A[1] */
    if (!A) {
        A_size = 2;                     /* indices 0 and 1 */
        A = (seq_t *)malloc(A_size * sizeof(seq_t));
        if (!A) return;
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;
    }

    if (k < A_size) return;              /* already computed */

    /* Grow the array to include index k */
    seq_t *tmp = (seq_t *)realloc(A, (k + 1) * sizeof(seq_t));
    if (!tmp) return;                    /* allocation failed – keep old data */
    A = tmp;

    /* Compute new entries from the current size up to k */
    for (idx_t i = A_size; i <= k; ++i) {
        if ((i & 1ULL) == 0ULL) {             /* even */
            idx_t m = i >> 1;                 /* m = i/2 */
            unsigned int shift = tz_count(m) + 2;
            seq_t diff = (((seq_t)1) << shift) - (seq_t)1;
            A[i] = A[i - 1] + diff;
        } else {                              /* odd */
            A[i] = A[i - 1] + (seq_t)1;
        }
    }
    A_size = k + 1;
}

/*--- main sequence ---------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 1, but safe */

    /* Determine k such that n matches one of the patterns */
    idx_t rem = n & 3ULL;              /* n % 4 */
    idx_t k;

    if (rem == 3ULL)          /* n = 4*k - 1  => k = (n+1)/4 */
        k = (n + 1) >> 2;
    else                       /* n = 4*k, 4*k+1, or 4*k+2 */
        k = n >> 2;           /* integer division truncates to floor */

    ensure_A(k);               /* make sure A[0..k] exist */

    /* sign sgn = (-1)^k */
    seq_t sgn = (k & 1ULL) ? (seq_t)-1 : (seq_t)1;
    seq_t result;

    switch (rem) {
        case 0ULL:                /* n = 4*k            */
            result = sgn * (((seq_t)2) * A[k]);
            break;
        case 1ULL:                /* n = 4*k + 1        */
            result = -sgn * (((seq_t)2 * A[k]) - (seq_t)1);
            break;
        case 2ULL:                /* n = 4*k + 2        */
            result = -sgn * (((seq_t)2 * A[k]) - (seq_t)2);
            break;
        case 3ULL:                /* n = 4*k - 1        */
            result = sgn;
            break;
        default:
            result = (seq_t)0;   /* unreachable */
            break;
    }
    return result;
}
