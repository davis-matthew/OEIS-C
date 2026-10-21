/*
 * A095163  a(n) = smallest divisor d of n that occurs earlier in the sequence
 *          fewer than d times.
 * https://oeis.org/A095163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n that has occurred earlier fewer
 *            than d times
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global cache ----------------------------------------------------- */
static seq_t *seq = NULL;     /* array of computed terms (0‑based) */
static size_t seq_len = 0;    /* number of terms stored          */
static size_t seq_cap = 0;    /* current capacity of seq array     */

static idx_t *cnt = NULL;     /* count of appearances of each value */
static size_t cnt_cap = 0;    /* current capacity of cnt array     */

/* --- helpers ----------------------------------------------------------- */

/* Integer square‑root for 64‑bit values, binary search with 128‑bit product. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Ensure the sequence cache holds at least `need` entries. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure the count cache holds indices up to `value` (inclusive). */
static void ensure_cnt_capacity(idx_t value)
{
    if ((size_t)value < cnt_cap)
        return;
    size_t new_cap = cnt_cap ? cnt_cap * 2 : 64;
    while (new_cap <= (size_t)value)
        new_cap <<= 1;
    idx_t *new_cnt = (idx_t *)realloc(cnt, new_cap * sizeof(idx_t));
    /* initialise new slots to zero */
    for (size_t i = cnt_cap; i < new_cap; ++i)
        new_cnt[i] = 0;
    cnt = new_cnt;
    cnt_cap = new_cap;
}

/* --------------------------------------------------------------------- */

/* Return a(n) for n ≥ 1.  The function is memoised; it can be called
   in any order and will compute any missing terms when needed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, return 0 for safety */

    /* compute terms up to n if not already cached */
    for (idx_t k = seq_len + 1; k <= n; ++k) {
        idx_t limit = isqrt_u64(k);
        idx_t best_d = k;      /* at least k is always a divisor */

        for (idx_t d = 1; d <= limit; ++d) {
            if (k % d == 0) {
                /* divisor d */
                ensure_cnt_capacity(d);
                if (cnt[d] < d && d < best_d)
                    best_d = d;

                /* paired divisor k/d */
                idx_t other = k / d;
                if (other != d) {
                    ensure_cnt_capacity(other);
                    if (cnt[other] < other && other < best_d)
                        best_d = other;
                }
            }
        }

        /* store computed term */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = (seq_t)best_d;

        /* update its count */
        ensure_cnt_capacity(best_d);
        cnt[best_d]++;
    }

    return seq[n - 1];   /* seq[0] holds a(1) */
}
