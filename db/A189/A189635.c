/*
 * A189635  Partial sums of A189632.
 * https://oeis.org/A189635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(n) with s(0) = 0, s(1) = 0, s(n) = s([ (n+2)/3 ]) + [
 *            n/3 ] – s([ n/3 ]) where [x] is ⌊x⌋. Valid for all n ≥ 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for previously computed s(k) values */
static seq_t *s_arr   = NULL;   /* array to hold s(k) for 0 ≤ k ≤ n */
static idx_t allocated = 0;     /* current allocated size (≥ n+1) */
static idx_t computed  = 0;     /* largest index whose s(k) has been computed */

/* Ensure we have room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* Need array size at least n+1 (for indices 0 … n) */
    if ((idx_t)(n + 1) <= allocated) return;

#ifdef __cplusplus
    /* In C++ use new[] / delete[] */
    seq_t *new_s = new seq_t[n + 1];
    if (s_arr)
    {
        for (idx_t i = 0; i < allocated; ++i) new_s[i] = s_arr[i];
        delete[] s_arr;
    }
    s_arr = new_s;
#else
    /* In C use realloc (realloc(NULL, ...) works as malloc) */
    seq_t *new_s = realloc(s_arr, (n + 1) * sizeof(seq_t));
    if (!new_s) return; /* allocation failed; keep old data */
    s_arr = new_s;
#endif
    allocated = n + 1;
}

/* Compute a(n) for arbitrary n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for n=0 case */

    /* Ensure storage for indices up to n */
    ensure_capacity(n);

    /* Initialize base values if not done yet */
    if (computed < 1)
    {
        s_arr[0] = (seq_t)0;
        if (allocated > 1) s_arr[1] = (seq_t)0;
        computed = (allocated > 1) ? 1 : 0;
    }

    /* Compute s(k) for k = computed+1 … n, in increasing order */
    idx_t start = (computed > 1) ? computed + 1 : 2;
    for (idx_t k = start; k <= n; ++k)
    {
        idx_t idx_a = (k + 2) / 3;   /* floor((k+2)/3)   */
        idx_t idx_b = k / 3;         /* floor(k/3)       */
        /* s(k) = s(idx_a) + idx_b - s(idx_b) */
        s_arr[k] = s_arr[idx_a] + (seq_t)idx_b - s_arr[idx_b];
    }

    computed = n;
    return s_arr[n];
}
