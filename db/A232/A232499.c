/*
 * A232499  Number of unit squares, aligned with a Cartesian grid, completely
 *          within the first quadrant of a circle centered at the origin
 *          ordered by increasing radius.
 * https://oeis.org/A232499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} R(s_k) where s_k are the integers that can be
 *            expressed as a^2 + b^2 with a,b ≥ 1, sorted increasingly and
 *            R(s) is the number of ordered pairs (a,b) giving that sum. a(n)
 *            is the cumulative sum of these counts. The function computes the
 *            sequence lazily and memoises results
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_arr = NULL;       /* a(1), a(2), …           */
static size_t seq_len = 0, seq_cap = 0;
static seq_t *cnt_arr = NULL;       /* counts for each s       */
static uint64_t last_limit = 0;     /* largest s processed     */
static seq_t running_sum = 0;       /* running total           */

/* Recompute the counts and cumulative sequence up to 'limit' (inclusive). */
static void recompute(uint64_t limit)
{
    /* Allocate and zero the count array. */
    cnt_arr = (seq_t *)realloc(cnt_arr, (limit + 1) * sizeof(seq_t));
    if (!cnt_arr) abort();
    for (size_t i = 0; i <= limit; ++i) cnt_arr[i] = 0;

    /* Count ordered pairs (a,b) with a,b ≥ 1 and a²+b² ≤ limit. */
    for (idx_t a = 1; (idx_t)a * a <= limit; ++a) {
        for (idx_t b = 1; (idx_t)a * a + (idx_t)b * b <= limit; ++b) {
            size_t s = (size_t)((idx_t)a * a + (idx_t)b * b);
            ++cnt_arr[s];
        }
    }

    /* Allocate space for the cumulative sequence. */
    seq_arr = (seq_t *)realloc(seq_arr, limit * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_cap = limit;
    seq_len = 0;
    running_sum = 0;

    /* Build cumulative sequence from the counts. */
    for (size_t s = 2; s <= limit; ++s) {
        if (cnt_arr[s] > 0) {
            running_sum += cnt_arr[s];
            seq_arr[seq_len++] = running_sum;
        }
    }
    last_limit = limit;
}

/* Ensure we have at least 'n' terms in seq_arr. */
static void ensure_terms(idx_t n)
{
    if (n <= (idx_t)seq_len) return;

    uint64_t limit = last_limit ? last_limit * 2 : 8;
    while (seq_len < (size_t)n) {
        recompute(limit);
        limit *= 2;
    }
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence starts at offset 1 */
    ensure_terms(n);
    return seq_arr[n - 1];
}
