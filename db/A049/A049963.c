/*
 * A049963  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 2 and a(3) = 4.
 * https://oeis.org/A049963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n-1} a(i) + a(m), where m = 2*n-2-2^(p+1) and
 *            p = ⌊log₂(n-1)⌋ (i.e. 2^p < n-1 ≤ 2^(p+1)). Initial values:
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: floor(log₂(x)) for x > 0, x is uint64_t */
static unsigned floor_log2_idx(idx_t x)
{
    unsigned r = 0;
    while (x >>= 1) r++;
    return r;
}

/* Cached sequence and prefix sums */
static seq_t *A_vals   = NULL;   /* a(1) … a(N) stored at indices 0 … N-1 */
static seq_t *prefix   = NULL;   /* prefix[i] = a(1)+…+a(i+1)  */
static idx_t  N_cached = 0;      /* number of terms currently cached */

/* Ensure arrays can hold at least 'need' terms */
static void ensure_capacity(idx_t need)
{
    if (N_cached >= need) return;

    idx_t new_size = (N_cached == 0) ? 8 : N_cached * 2;
    while (new_size < need) new_size <<= 1;

    A_vals   = (seq_t*)realloc(A_vals,   new_size * sizeof(seq_t));
    prefix   = (seq_t*)realloc(prefix,   new_size * sizeof(seq_t));

    /* Initialise the first three terms on first allocation */
    if (N_cached == 0) {
        A_vals[0] = (seq_t)1;  /* a(1) */
        A_vals[1] = (seq_t)2;  /* a(2) */
        A_vals[2] = (seq_t)4;  /* a(3) */

        prefix[0] = A_vals[0];
        prefix[1] = prefix[0] + A_vals[1];
        prefix[2] = prefix[1] + A_vals[2];

        N_cached = 3;
    }
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* off‑range, not used by OEIS */

    /* Extend cache if necessary */
    ensure_capacity(n);

    /* If already computed, return it */
    if (n <= N_cached) return A_vals[n-1];

    /* Compute missing terms up to n */
    for (idx_t i = N_cached + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 3) {            /* already initialised */
            val = A_vals[i-1];
        } else {
            idx_t p      = floor_log2_idx(i-2);
            idx_t pow2p1 = (idx_t)1 << (p + 1);
            idx_t m      = (idx_t)2 * i - 2 - pow2p1;   /* m < i */

            /* sum_{k=1}^{i-1} a(k) = prefix[i-2] */
            val = prefix[i-2] + A_vals[m-1];
        }
        A_vals[i-1] = val;
        if (i == 1) {
            prefix[i-1] = val;
        } else {
            prefix[i-1] = prefix[i-2] + val;
        }
    }

    N_cached = n;
    return A_vals[n-1];
}
