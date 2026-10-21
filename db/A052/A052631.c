/*
 * A052631  a(n) = n!*Pell(n) (or n!*A000129(n)).
 * https://oeis.org/A052631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Pell(n) * Pell(0) = 0, Pell(1) = 1, Pell(n) =
 *            2*Pell(n-1) + Pell(n-2) * a(0) = 0, a(1) = 1 * Implementation
 *            uses simple O(n) forward computation with memoisation up to a
 *            fixed limit. * For n > 10000 the value is computed on the fly
 *            without storing it beyond the call. * The routine wor...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* maximum precomputed index for static caching */
    static const idx_t MAX_PRECOMPUTE = 10000;
    static idx_t max_precomputed = 0;
    /* static arrays for caching values up to MAX_PRECOMPUTE */
    static seq_t *A      = NULL;
    static seq_t *Pell   = NULL;
    static seq_t *Fact   = NULL;

    /* Allocate once for the static cache */
    if (A == NULL) {
        A     = (seq_t *)malloc((MAX_PRECOMPUTE + 1) * sizeof(seq_t));
        Pell  = (seq_t *)malloc((MAX_PRECOMPUTE + 1) * sizeof(seq_t));
        Fact  = (seq_t *)malloc((MAX_PRECOMPUTE + 1) * sizeof(seq_t));
        /* Initialise the base cases */
        Fact[0] = (seq_t)1;  /* 0! = 1 */
        Pell[0] = (seq_t)0;  /* Pell(0) */
        A[0]    = (seq_t)0;  /* a(0) = 0 */
        if (MAX_PRECOMPUTE >= 1) {
            Fact[1] = (seq_t)1;          /* 1! = 1 */
            Pell[1] = (seq_t)1;          /* Pell(1) */
            A[1]    = Fact[1] * Pell[1]; /* a(1) = 1 */
            max_precomputed = 1;
        }
    }

    /* If the requested index is within the cached range, extend it if necessary */
    if (n <= MAX_PRECOMPUTE) {
        if (n > max_precomputed) {
            for (idx_t k = max_precomputed + 1; k <= n; ++k) {
                Fact[k] = Fact[k - 1] * (seq_t)k;
                Pell[k] = (seq_t)2 * Pell[k - 1] + Pell[k - 2];
                A[k]    = Fact[k] * Pell[k];
            }
            max_precomputed = n;
        }
        return A[n];
    }

    /* For n > MAX_PRECOMPUTE, compute directly without caching */
    seq_t fact = ((n > 0) ? Fact[max_precomputed] : (seq_t)1);
    seq_t pell_prevprev = Pell[max_precomputed >= 1 ? max_precomputed - 1 : 0];
    seq_t pell_prev    = (max_precomputed >= 1 ? Pell[max_precomputed] : (seq_t)0);
    seq_t pell_cur;

    /* Compute factorial and Pell iteratively up to n */
    for (idx_t k = max_precomputed + 1; k <= n; ++k) {
        fact = fact * (seq_t)k;
        if (k == 1) {
            pell_cur = (seq_t)1;
        } else {
            pell_cur = (seq_t)2 * pell_prev + pell_prevprev;
        }
        pell_prevprev = pell_prev;
        pell_prev     = pell_cur;
    }

    return fact * pell_cur;
}
