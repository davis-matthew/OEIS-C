/*
 * A026470  a(n) = least positive integer > a(n-1) and not a(i)^2 + a(j)^2 for
 *          1<=i<=j<=n.
 * https://oeis.org/A026470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) and not equal to a(i)^2 +
 *            a(j)^2 for 1 ≤ i ≤ j ≤ n. Initial value a(1)=2. Implements
 *            greedy search
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static unsigned char *sum_flags = NULL;
static const idx_t SUM_CAP = 20000000ULL; /* maximum sum index we store */

static inline void grow_seq_if_needed(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 128 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = new_arr;
        seq_cap = new_cap;
    }
}

static inline void ensure_sum_flags(void)
{
    if (!sum_flags) {
        sum_flags = (unsigned char *)calloc(SUM_CAP + 1, 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    ensure_sum_flags();
    while (seq_len < (size_t)n) {
        seq_t cand;
        if (seq_len == 0) {
            cand = (seq_t)2;
        } else {
            cand = seq[seq_len - 1] + (seq_t)1;
        }

        /* find smallest candidate not marked as a sum of two squares */
        while (cand <= SUM_CAP && sum_flags[(idx_t)cand]) {
            cand++;
        }

        /* append the new term */
        grow_seq_if_needed();
        seq[seq_len++] = cand;

        /* mark all sums involving the new term up to SUM_CAP */
        unsigned long long sq_cand_ull = (unsigned long long)cand * (unsigned long long)cand;
        if (sq_cand_ull <= (unsigned long long)SUM_CAP) {
            seq_t sq_cand = (seq_t)sq_cand_ull;

            /* self sum */
            unsigned long long self_sum_ull = 2ULL * sq_cand_ull;
            if (self_sum_ull <= (unsigned long long)SUM_CAP) {
                sum_flags[(idx_t)self_sum_ull] = 1;
            }

            /* sums with all previous elements */
            for (size_t i = 0; i < seq_len - 1; ++i) {
                unsigned long long sq_prev_ull = (unsigned long long)seq[i] * (unsigned long long)seq[i];
                unsigned long long total = sq_cand_ull + sq_prev_ull;
                if (total <= (unsigned long long)SUM_CAP) {
                    sum_flags[(idx_t)total] = 1;
                }
            }
        }
        /* if sq_cand_ull > SUM_CAP all sums exceed the cap; nothing to mark */
    }
    return seq[n - 1];
}
