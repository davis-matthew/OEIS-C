/*
 * A123256  Dimension of the invariant subspace in modules over the symmetric
 *          groups S_n of dimension n*(n+1)^(n-1).
 * https://oeis.org/A123256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = sum_{d | (n-1)} A000081(d) where A000081 is the number of
 *            rooted unlabeled trees on d nodes. A000081(1)=1 and for m>1
 *            satisfies A000081(m) = (1/(m-1)) * sum_{k=1}^{m-1} ( sum_{d | k}
 *            d*A000081(d) ) * A000081(m-k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* IMPORTANT: external linkage, exact signature required. */
/* Note: seq_t and idx_t are defined elsewhere; no inclusion here. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

static seq_t *A0 = NULL;   /* A000081 values, indexed from 1 */
static seq_t *T  = NULL;   /* t[k] = sum_{d | k} d*A000081(d) */
static idx_t   A0cnt = 0;  /* largest k with A0[k] computed */
static idx_t   Tcnt   = 0; /* largest k with T[k]   computed */

/* Ensure we have values of A000081 and T up to at least n */
static void ensure_root(idx_t n)
{
    if (n <= A0cnt) return;              /* already computed */

    /* Allocate or grow arrays to size n+1 (index from 1). */
    if (A0 == NULL) {
        A0 = (seq_t *)malloc((n+2) * sizeof(seq_t));
        T  = (seq_t *)malloc((n+2) * sizeof(seq_t));
        if (!A0 || !T) return;           /* allocation failure (not handled) */
        A0cnt = 0; Tcnt = 0;
    } else {
        A0 = (seq_t *)realloc(A0, (n+2) * sizeof(seq_t));
        T  = (seq_t *)realloc(T,  (n+2) * sizeof(seq_t));
        if (!A0 || !T) return;           /* allocation failure (not handled) */
    }

    /* Initialise base case if needed */
    if (A0cnt == 0) {
        A0[1] = 1;
        T[1]  = 1 * A0[1];
        A0cnt = 1; Tcnt = 1;
    }

    /* Compute successive terms up to n */
    for (idx_t m = A0cnt + 1; m <= n; ++m) {
        /* Compute convolution sum: sum_{k=1}^{m-1} T[k] * A0[m-k] */
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            sum += T[k] * A0[m-k];
        }
        /* Divide by (m-1); division is exact by theory */
        A0[m] = sum / (m-1);

        /* Compute T[m] = sum_{d | m} d * A0[d] */
        seq_t tm = 0;
        idx_t r = (idx_t)sqrt((double)m);
        for (idx_t d = 1; d <= r; ++d) {
            if (m % d == 0) {
                tm += (seq_t)d * A0[d];
                idx_t d2 = m / d;
                if (d2 != d) tm += (seq_t)d2 * A0[d2];
            }
        }
        T[m] = tm;
    }
    A0cnt = n;
    Tcnt  = n;   /* T up to n is not needed beyond but keep in sync */
}

/* Return a(n) for n >= 2 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour for offset is irrelevant */
    idx_t m = n - 1;              /* we need divisors of m */

    /* Ensure A000081 up to m */
    ensure_root(m);

    /* Sum A0[d] over all divisors d of m */
    seq_t res = 0;
    idx_t r = (idx_t)sqrt((double)m);
    for (idx_t d = 1; d <= r; ++d) {
        if (m % d == 0) {
            res += A0[d];
            idx_t d2 = m / d;
            if (d2 != d) res += A0[d2];
        }
    }
    return res;
}
