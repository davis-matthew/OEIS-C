/*
 * A212338  Sequence of coefficients of x in marked mesh pattern generating
 *          function Q_{n,132}^(0,0,2,0)(x).
 * https://oeis.org/A212338
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 78, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212338
   Formula: a(n) = 2*A001628(n-3) + A001628(n-4)   (offset 3)
   where A001628 satisfies
        G[0] = 1, G[1] = 3, G[2] = 9, G[3] = 22, G[4] = 51, G[5] = 111,
        G[n] = 3*G[n-1] - 5*G[n-3] + 3*G[n-5] + G[n-6]   (n >= 6) */

#include <stddef.h>
#include <stdlib.h>

/* caching of G values: G[k] = A001628(k) */
static seq_t *G = NULL;      /* pointer to array of G values          */
static idx_t Glen = 0;       /* number of valid G entries (>=0)       */

/* expand G so that indices up to 'upto' are available (upto >= 0). */
static void ensure_G(idx_t upto)
{
    /* allocate or grow the array */
    if (G == NULL) {
        G = (seq_t *)malloc((size_t)(upto + 1) * sizeof(seq_t));
    } else if (Glen <= upto) {
        seq_t *tmp = (seq_t *)realloc(G, (size_t)(upto + 1) * sizeof(seq_t));
        if (tmp) G = tmp;
    }

    /* compute missing entries */
    for (idx_t i = Glen; i <= upto; ++i) {
        if (i == 0)          G[i] = 1;
        else if (i == 1)     G[i] = 3;
        else if (i == 2)     G[i] = 9;
        else if (i == 3)     G[i] = 22;
        else if (i == 4)     G[i] = 51;
        else if (i == 5)     G[i] = 111;
        else { /* recurrence for i >= 6 */
            seq_t v = G[i-1] * 3 - G[i-3] * 5 + G[i-5] * 3 + G[i-6];
            G[i] = v;
        }
    }
    Glen = upto + 1;
}

seq_t a(idx_t n)
{
    if (n < 3)                   /* undefined below the offset */
        return 0;

    if (n == 3)                  /* a(3) = 2 is a special case */
        return 2;

    /* we need G[n-3] and G[n-4] (n >= 4 => n-4 >= 0) */
    ensure_G(n - 3);

    seq_t g3 = G[n - 3];   /* A001628(n-3) */
    seq_t g4 = G[n - 4];   /* A001628(n-4) */
    return g3 * 2 + g4;
}
