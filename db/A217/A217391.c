/*
 * A217391  Partial sums of the squares of the ordered Bell numbers (number of
 *          preferential arrangements) A000670.
 * https://oeis.org/A217391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} t(k)^2, where t(k) = A000670 (ordered Bell
 *            numbers, number of preferential arrangements)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised arrays of the ordered Bell numbers and the partial sums
   of their squares. */
static seq_t *t_vals   = NULL;   /* t(k)  */
static seq_t *sum_vals = NULL;   /* Σ_{i=0..k} t(i)^2 */
static size_t cap      = 0;      /* current allocation size          */
static size_t max_n    = (size_t)-1;  /* last n for which values are known */

/* Grow the internal arrays to accommodate at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap) return;
    size_t new_size = new_cap > 2*cap ? new_cap : (cap ? 2*cap : 1);
#ifdef __cplusplus
    t_vals   = (seq_t*)realloc(t_vals,   new_size * sizeof(seq_t));
    sum_vals = (seq_t*)realloc(sum_vals, new_size * sizeof(seq_t));
#else
    t_vals   = realloc(t_vals,   new_size * sizeof(seq_t));
    sum_vals = realloc(sum_vals, new_size * sizeof(seq_t));
#endif
    cap = new_size;
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Initialise the base case if this is the first call. */
    if (max_n == (size_t)-1) {
        ensure_capacity(1);
        t_vals[0]   = (seq_t)1;
        sum_vals[0] = (seq_t)1;          /* 1^2 = 1 */
        max_n = 0;
    }

    /* Compute values up to N if necessary. */
    if (N > max_n) {
        ensure_capacity(N + 1);
        for (size_t i = max_n + 1; i <= N; ++i) {
            seq_t s = (seq_t)0;
            seq_t bin = (seq_t)1;   /* C(i,0) */
            for (size_t k = 0; k < i; ++k) {
                s += bin * t_vals[k];
                if (k + 1 < i) {
                    /* Next binomial coefficient: C(i,k+1) = C(i,k)*(i-k)/(k+1) */
                    bin = (bin * (seq_t)(i - k)) / (seq_t)(k + 1);
                }
            }
            t_vals[i] = s;
            seq_t sq = s * s;
            sum_vals[i] = (i == 0 ? sq : sum_vals[i-1] + sq);
        }
        max_n = N;
    }

    return sum_vals[N];
}
