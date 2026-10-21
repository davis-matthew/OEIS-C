/*
 * A230855  Numbers n such that m + (sum of digits in base-3 representation of
 *          m) = n has exactly three solutions.
 * https://oeis.org/A230855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 5162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that `m + digit_sum_base3(m) = n` has
   exactly three solutions. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state: counts of preimages, the result list, and
   control variables for the incremental scan. */
static uint16_t *cnt          = NULL;     /* counts for each n */
static size_t   cnt_cap       = 0;        /* capacity of `cnt` */
static idx_t    finalized_n   = 0;        /* largest n that has been finalized */

static seq_t *results          = NULL;     /* array of n's with exactly 3 preimages */
static size_t res_cap          = 0;        /* capacity of `results` */
static size_t res_size         = 0;        /* number of stored terms */

static idx_t   next_m          = 0;        /* next m to be processed */

/* Ensure `cnt` can hold indices up to `min_cap-1`. */
static void ensure_cnt_cap(idx_t min_cap)
{
    if ((idx_t)cnt_cap >= min_cap) return;
    size_t old_cap = cnt_cap;
    size_t new_cap = cnt_cap ? cnt_cap * 2 : 1024;
    if (new_cap < (size_t)min_cap) new_cap = (size_t)min_cap;
    uint16_t *tmp = (uint16_t*)realloc(cnt, new_cap * sizeof(uint16_t));
    if (!tmp) abort();
    cnt = tmp;
    for (size_t i = old_cap; i < new_cap; ++i) cnt[i] = 0;
    cnt_cap = new_cap;
}

/* Ensure `results` has at least `min_cap` elements. */
static void ensure_res_cap(size_t min_cap)
{
    if (res_cap >= min_cap) return;
    size_t new_cap = res_cap ? res_cap * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    results = tmp;
    res_cap = new_cap;
}

/* Finalise all n ≤ up_to: if cnt[n] == 3, append n to `results`. */
static void finalize_up_to(idx_t up_to)
{
    if (up_to <= finalized_n) return;
    ensure_cnt_cap(up_to + 1);
    for (idx_t n = finalized_n + 1; n <= up_to; ++n) {
        if (cnt[n] == 3) {
            ensure_res_cap(res_size + 1);
            results[res_size++] = (seq_t)n;
        }
    }
    finalized_n = up_to;
}

/* Sum of the digits of x in base‑3. */
static idx_t sum_digits_base3(idx_t x)
{
    idx_t s = 0;
    while (x) {
        s += x % 3;
        x /= 3;
    }
    return s;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    while (res_size < (size_t)n) {
        idx_t m = next_m;
        idx_t d = sum_digits_base3(m);
        idx_t n_val = m + d;

        ensure_cnt_cap(n_val + 1);
        ++cnt[n_val];

        ++next_m;                 /* move to next m */
        finalize_up_to(next_m);   /* finalize all n ≤ next_m */
    }
    return results[n - 1];
}
