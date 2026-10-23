/*
 * A160979  Number of partitions of n where every part appears at least 9
 *          times.
 * https://oeis.org/A160979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 191, u32 n <= 514, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160979: number of partitions of n in which every part appears at least 9 times. */

/* The DP keeps two arrays `dp_old` and `dp_new` of length `max+1`.  For each part
 * size k (1 ≤ k ≤ max/9) we multiply the current generating function by
 * 1 + x^{9k} + x^{10k} + … .  For a residue class r (mod k) the update
 * reduces to
 *
 *   new[r + t·k] = old[r + t·k]  +  Σ_{u=9}^{t} old[r + (t-u)·k]     (t ≥ 0)
 *
 * The prefix‑sum trick turns this into an O(max²/9) algorithm, fast enough for
 * n ≤ 10000.  All arithmetic is done in type seq_t.  The function is memoised
 * and can be called in any order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Auxiliary allocator/destructor that works for both integer and
   arbitrary‑precision seq_t types. */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n]();
#else
    return (seq_t *)calloc(n, sizeof(seq_t));
#endif
}
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* Static state used for memoisation. */
static seq_t *dp_old   = NULL;   /* partition counts for the current processed k */
static seq_t *dp_new   = NULL;   /* scratch buffer for the next k */
static seq_t *tmp_arr  = NULL;   /* values of a single residue class */
static seq_t *pref_arr = NULL;   /* prefix sums of tmp_arr */
static idx_t cache_max = 0;      /* largest n for which dp_old is valid */

/* Recompute the table up to `max` (inclusive). */
static void recompute(idx_t max)
{
    /* free previous storage */
    if (dp_old)   free_seq_array(dp_old);
    if (dp_new)   free_seq_array(dp_new);
    if (tmp_arr)  free_seq_array(tmp_arr);
    if (pref_arr) free_seq_array(pref_arr);

    /* allocate new buffers */
    size_t sz = (size_t)max + 1;
    dp_old   = alloc_seq_array(sz);
    dp_new   = alloc_seq_array(sz);
    tmp_arr  = alloc_seq_array(sz);
    pref_arr = alloc_seq_array(sz);

    /* initialise dp_old: only the empty partition contributes */
    for (size_t i = 0; i < sz; ++i) dp_old[i] = (seq_t)0;
    dp_old[0] = (seq_t)1;

    idx_t limit = max / 9;                         /* only parts divisible by 9 may appear ≥9 times */

    for (idx_t k = 1; k <= limit; ++k) {
        /* process all r mod k */
        for (idx_t r = 0; r < k; ++r) {
            if (r > max) break;                   /* no numbers with this residue */
            idx_t T = (max - r) / k;              /* largest t s.t. r + t·k ≤ max */

            /* copy values of old DP in this residue class */
            for (idx_t t = 0; t <= T; ++t)
                tmp_arr[t] = dp_old[r + t * k];

            /* build prefix sums of the copied array */
            pref_arr[0] = tmp_arr[0];
            for (idx_t t = 1; t <= T; ++t)
                pref_arr[t] = pref_arr[t - 1] + tmp_arr[t];

            /* compute new values for this residue class */
            for (idx_t t = 0; t <= T; ++t) {
                seq_t val;
                if (t < 9)             /* only the empty choice */
                    val = tmp_arr[t];
                else
                    val = tmp_arr[t] + pref_arr[t - 9];
                dp_new[r + t * k] = val;
            }
        }
        /* swap buffers: next iteration uses the newly computed values */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }
    cache_max = max;
}

/* Public interface.  Memoised to avoid recomputation for already covered n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset value not required */
    if (n > cache_max) recompute(n);
    return dp_old[n];
}
