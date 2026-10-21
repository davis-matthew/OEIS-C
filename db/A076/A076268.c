/*
 * A076268  Sum(k=1,n, A005185(k)).
 * https://oeis.org/A076268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} A005185(k) (partial sums of the Hofstadter
 *            Q‑sequence)
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system (e.g. uint64_t, unsigned __int128, arbitrary‑precision) */
static seq_t *Q      = NULL;   /* Q(i) stored at Q[i-1]  */
static seq_t *cum    = NULL;   /* cumulative sum a(i)   */
static idx_t  computed = 0;    /* number of terms computed so far   */
static idx_t  capacity  = 0;    /* current allocation size   */

/* Ensure arrays have capacity for indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if (n + 1 <= capacity) return;
    idx_t new_cap = (n + 1) * 2;            /* grow to reduce reallocs */
    if (!Q) {
        Q   = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
        cum = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
    } else {
        Q   = (seq_t *)realloc(Q,   new_cap * sizeof(seq_t));
        cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
    }
    /* If realloc failed the program will abort – acceptable for this context. */
    capacity = new_cap;
}

/* Compute a(n) = Σ_{k=1}^{n} A005185(k) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, safe fallback */

    if (computed >= n) return cum[n-1];       /* already computed */

    ensure_size(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1 || i == 2) {
            val = 1;
        } else {
            /* Q(i) = Q( i - Q(i-1) ) + Q( i - Q(i-2) )  */
            idx_t idx1 = i - (idx_t)Q[i-2];   /* Q(i-1) stored at Q[i-2] */
            idx_t idx2 = i - (idx_t)Q[i-3];   /* Q(i-2) stored at Q[i-3] */
            /* idx1, idx2 are guaranteed ≥ 1 for the Hofstadter Q‑sequence */
            val = Q[idx1-1] + Q[idx2-1];
        }
        Q[i-1] = val;
        if (i == 1) {
            cum[0] = val;
        } else {
            cum[i-1] = cum[i-2] + val;
        }
    }
    computed = n;
    return cum[n-1];
}
