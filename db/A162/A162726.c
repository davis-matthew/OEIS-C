/*
 * A162726  G.f. is the polynomial (Product_{k=1..27} (1 - x^(3*k)))/(1-x)^27.
 * https://oeis.org/A162726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..27} (1 - x^(3*k))/(1-x)^27.
 *            This equals ∏_{k=1..27} (1 + x + … + x^(3*k-1)), a polynomial of
 *            degree 1107 whose coefficients for n>1107 are 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 45, u128 n <= 497,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq = NULL;        /* cache of computed coefficients */
static idx_t  seq_cap = 0;       /* size of seq array (MAX_DEG + 1) */

static void init_seq(void)
{
    const idx_t MAX_DEG = 1107;          /* highest n with a(n)≠0   */
    idx_t i;
    /* temporary working buffers */
    seq_t *buf1 = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    seq_t *buf2 = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    seq_t *a = buf1;                      /* current coefficients   */
    seq_t *b = buf2;                      /* buffer for next step   */
    /* initialise a to the constant term of 1/(1-x)^27 */
    for (i = 0; i <= MAX_DEG; ++i) a[i] = 0;
    a[0] = 1;

    /* sequentially multiply by (1 + x + … + x^(3*k‑1)) for k=1..27 */
    for (idx_t k = 1; k <= 27; ++k) {
        idx_t L = 3 * k;                 /* length of the all‑one kernel */

        /* prefix sums of a   (pref[n] = a[0] + … + a[n]) */
        pref[0] = a[0];
        for (i = 1; i <= MAX_DEG; ++i) pref[i] = pref[i-1] + a[i];

        /* conv: b[n] = Σ_{t=0}^{L-1} a[n‑t] = pref[n] – pref[n‑L] */
        for (i = 0; i <= MAX_DEG; ++i) {
            seq_t high = (i >= L) ? pref[i-L] : (seq_t)0;
            b[i] = pref[i] - high;
        }

        /* swap a and b for next iteration */
        seq_t *tmp = a;
        a = b;
        b = tmp;
    }

    /* store final result in seq */
    seq = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    for (i = 0; i <= MAX_DEG; ++i) seq[i] = a[i];
    seq_cap = MAX_DEG + 1;

    /* free temporary buffers */
    free(buf1);
    free(buf2);
    free(pref);
}

/* Sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) { init_seq(); initialized = 1; }
    if (n + 1 > seq_cap) return 0;
    return seq[n];
}
