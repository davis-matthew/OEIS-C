/*
 * A046201  Distinct odd numbers in the triangle of denominators in Leibniz's
 *          Harmonic Triangle.
 * https://oeis.org/A046201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc, seq_cmp, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate odd denominators of Leibniz's Harmonic Triangle in order of appearance */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_cmp.h"

static size_t lower_bound_seq(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq_cmp(&arr[mid], &val) < 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

static void record_value(seq_t val, seq_t **seq_cache, size_t *seq_len, size_t *seq_cap,
                         seq_t **seen_cache, size_t *seen_len, size_t *seen_cap)
{
    size_t pos = lower_bound_seq(*seen_cache, *seen_len, val);
    if (pos == *seen_len || seq_cmp(&(*seen_cache)[pos], &val) != 0) {
        /* insert into seen_cache */
        if (*seen_len == *seen_cap) {
            size_t new_cap = (*seen_cap == 0) ? 16 : (*seen_cap << 1);
            *seen_cache = seq_realloc(*seen_cache, *seen_cap, new_cap);
            *seen_cap = new_cap;
        }
        for (size_t j = *seen_len; j > pos; --j)
            (*seen_cache)[j] = (*seen_cache)[j - 1];
        (*seen_cache)[pos] = val;
        ++(*seen_len);

        /* add to sequence cache */
        if (*seq_len == *seq_cap) {
            size_t new_cap = (*seq_cap == 0) ? 16 : (*seq_cap << 1);
            *seq_cache = seq_realloc(*seq_cache, *seq_cap, new_cap);
            *seq_cap = new_cap;
        }
        (*seq_cache)[*seq_len] = val;
        ++(*seq_len);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for index < 1 */

    static seq_t *seq_cache = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *seen_cache = NULL;
    static size_t seen_len = 0, seen_cap = 0;
    static idx_t next_row = 1;

    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = seq_alloc(seq_cap);
    }
    if (seen_cache == NULL) {
        seen_cap = 16;
        seen_cache = seq_alloc(seen_cap);
    }

    while (seq_len < (size_t)n) {
        for (idx_t r = next_row; seq_len < (size_t)n; ++r) {
            if ((r & (idx_t)1) == 0) continue; /* even rows produce only even denominators */

            idx_t n1 = r - 1;
            seq_t binom = (seq_t)1; /* C(n1, 0) */
            /* k = 0 */
            if (seq_len < (size_t)n && (binom & (seq_t)1) != 0) {
                seq_t denom = (seq_t)r * binom;
                record_value(denom, &seq_cache, &seq_len, &seq_cap, &seen_cache, &seen_len, &seen_cap);
            }
            /* k from 1 to n1 */
            for (idx_t k = 1; k <= n1; ++k) {
                /* compute next binomial coefficient */
                binom = binom * (seq_t)(n1 - k + 1) / (seq_t)k;
                if ((binom & (seq_t)1) != 0) {
                    seq_t denom = (seq_t)r * binom;
                    record_value(denom, &seq_cache, &seq_len, &seq_cap, &seen_cache, &seen_len, &seen_cap);
                }
                if (seq_len >= (size_t)n) break;
            }
        }
        /* After the loop, r has been incremented one past the last used row */
        next_row = next_row; /* next_row already advanced inside loop */
    }

    return seq_cache[n - 1];
}
