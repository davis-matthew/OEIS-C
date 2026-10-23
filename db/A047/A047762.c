/*
 * A047762  Number of chiral pairs of dissectable polyhedra with n tetrahedral
 *          cells and symmetry of type E.
 * https://oeis.org/A047762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for even n; for odd n, * a(n) = 1/4 * (A047749(n) -
 *            2*A047760(n) - 6*A047758(n) - 2*A047754(n) * - 3*A047753(n) -
 *            2*A047752(n) - A047751(n)) * * The following implementation
 *            evaluates the sequence via the * generating‑function
 *            description: * * G(z) = 1 + z·G(z)^3 (generating function of...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- Static state ---------------------------------------------------*/
static seq_t *G  = NULL;          /* G[0..cap-1]   */
static idx_t  G_cap = 0;

static seq_t *G2 = NULL;          /* G2[0..cap-1]  (G * G) */
static idx_t  G2_cap = 0;

/*--- Compute G[0..need] ----------------------------------------------*/
static void ensure_G(idx_t need)
{
    if (G_cap > need) return;

    /* Allocate or enlarge storage. */
    seq_t *tmp = (seq_t*)realloc(G, (need + 1) * sizeof(seq_t));
    if (!tmp) return;               /* allocation failure → ignore   */
    G = tmp;

    /* Initialise first element if it was not created yet. */
    if (G_cap == 0) {
        G[0] = (seq_t)1;
        G_cap = 1;
    }

    /* Compute new coefficients.  G[0] is already known. */
    for (idx_t n = G_cap; n <= need; ++n) {
        /* binomial(3n, n‑1) / n  (valid for n >= 1) */
        seq_t binom = (seq_t)1;
        for (idx_t i = 1; i <= n - 1; ++i) {
            binom = binom * (((seq_t)(3 * n)) - (seq_t)i + 1);
            binom = binom / (seq_t)i;   /* exact division */
        }
        G[n] = binom / (seq_t)n;
    }
    G_cap = need + 1;
}

/*--- Compute G2[0..need] (= convolution G*G) ---------------------------*/
static void ensure_G2(idx_t need)
{
    if (G2_cap > need) return;

    seq_t *tmp = (seq_t*)realloc(G2, (need + 1) * sizeof(seq_t));
    if (!tmp) return;
    G2 = tmp;

    for (idx_t k = 0; k <= need; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i) {
            sum += G[i] * G[k - i];
        }
        G2[k] = sum;
    }
    G2_cap = need + 1;
}

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* Even indices yield zero. */
    if ((n & (idx_t)1) == 0) return (seq_t)0;

    /* N = n + 1   (even because n is odd) */
    idx_t N = n + (idx_t)1;
    if (N < 2) return (seq_t)0;   /* safety, though n >= 1 guarantees N >= 2 */

    /* Determine maximal index required among all terms. */
    idx_t need_G = (N >= 2) ? ((N - 2) / 2) : (idx_t)0;
    ensure_G(need_G);
    ensure_G2(need_G);

    seq_t num = (seq_t)0;

    /* 1. z^2 * G(z^2)^2   → coefficient of z^N is G2[(N-2)/2] if N-2 even */
    idx_t M = N - 2;
    if ((M & (idx_t)1) == 0) {
        idx_t t = M / 2;
        if (t <= need_G) num += G2[t];
    }

    /* 2. -(2+z^2) * G(z^4) */
    if (N % 4 == 0) {
        idx_t idx = N / 4;
        if (idx <= need_G) num -= (seq_t)2 * G[idx];
    }
    if (N >= 2 && (N - 2) % 4 == 0) {
        idx_t idx = (N - 2) / 4;
        if (idx <= need_G) num -= G[idx];
    }

    /* 3. -2*z^2 * G(z^4)^2 */
    if (N >= 2 && (N - 2) % 4 == 0) {
        idx_t t = (N - 2) / 4;
        if (t <= need_G) num -= (seq_t)2 * G2[t];
    }

    /* 4. +2 * (1 + z^2*G(z^8) + z^6*G(z^8)^2) */
    /*    The constant term 2*1 contributes only to coefficient 0, which we
     *    are not interested in (N ≥ 2). */
    if (N >= 2 && (N - 2) % 8 == 0) {
        idx_t idx = (N - 2) / 8;
        if (idx <= need_G) num += (seq_t)2 * G[idx];
    }
    if (N >= 6 && (N - 6) % 8 == 0) {
        idx_t t = (N - 6) / 8;
        if (t <= need_G) num += (seq_t)2 * G2[t];
    }

    /* Final division by 4 (exact). */
    return num / (seq_t)4;
}
