/*
 * A080432  Even numbers such that all a(i) + a(j) are distinct.
 * https://oeis.org/A080432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction: a(1)=2; for n>1 take the smallest even integer that
   does not produce a duplicate sum with any earlier elements. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helpers ------------------------------------------------------------------*/
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 4;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

static uint64_t *sum_bits = NULL;
static size_t sum_bits_cap = 0;     /* number of bits stored */

static void ensure_sum_bits(idx_t need)
{
    idx_t required = need + 1ULL;   /* inclusive */
    if ((size_t)required <= sum_bits_cap) return;
    size_t new_words = (required + 63ULL) / 64ULL;
    size_t old_words = (sum_bits_cap + 63ULL) / 64ULL;
    uint64_t *tmp = (uint64_t*)realloc(sum_bits, new_words * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (new_words > old_words)
        memset(tmp + old_words, 0, (new_words - old_words) * sizeof(uint64_t));
    sum_bits = tmp;
    sum_bits_cap = new_words * 64ULL;
}

static inline bool sum_bit_get(idx_t pos)
{
    return (sum_bits[pos >> 6] >> (pos & 63)) & 1ULL;
}

static inline void sum_bit_set(idx_t pos)
{
    sum_bits[pos >> 6] |= 1ULL << (pos & 63);
}

/* Sequence state -----------------------------------------------------------*/
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Main function ------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below the offset */

    while (seq_len < n) {
        grow_seq(&seq, &seq_cap, seq_len + 1);

        /* candidate: next even number after the last element */
        seq_t cand = (seq_len == 0) ? (seq_t)2 : seq[seq_len - 1] + (seq_t)2;

        /* find first admissible candidate */
        for (;;) {
            bool conflict = false;
            for (idx_t i = 0; i < seq_len; ++i) {
                idx_t sum = (idx_t)seq[i] + (idx_t)cand;
                ensure_sum_bits(sum);
                if (sum_bit_get(sum)) { conflict = true; break; }
            }
            if (!conflict) break;
            cand += (seq_t)2;
        }

        /* accept candidate and record its sums */
        seq[seq_len] = cand;
        for (idx_t i = 0; i < seq_len; ++i) {
            idx_t sum = (idx_t)seq[i] + (idx_t)cand;
            ensure_sum_bits(sum);
            sum_bit_set(sum);
        }
        seq_len++;
    }
    return seq[n - 1];
}
