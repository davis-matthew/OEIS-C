/*
 * A025355  Numbers that are the sum of 3 distinct nonzero squares in 9 or
 *          more ways.
 * https://oeis.org/A025355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth positive integer that can be written as the sum
   of three DISTINCT positive squares in at least nine different unordered
   ways.  The implementation builds a sieve of all ordered triples of
   squares up to a dynamic maximum sum, then extracts those sums whose
   unordered count of distinct triples is ≥ 9.  The list of qualifying
   numbers is kept in a growing static array, so a(n) can be queried in
   any order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Integer square root for 64‑bit unsigned values (floor).               */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --------------------------------------------------------------------- */
/* Internal state ------------------------------------------------------ */
static idx_t      current_max_sum = 0;   /* largest sum processed   */
static seq_t     *seq_list         = NULL;/* sorted qualifying numbers */
static size_t    seq_len          = 0;    /* number of stored values */

/* --------------------------------------------------------------------- */
/* Rebuild list of qualifying numbers up to new_max_sum (must be >
   current_max_sum).                                                    */
static void recompute(idx_t new_max_sum)
{
    if (new_max_sum <= current_max_sum) return;

    /* Number of square values that are <= new_max_sum   */
    size_t sq_cnt = (size_t)isqrt_u64(new_max_sum);
    if (sq_cnt == 0) return;

    /* Build the list of squares (non‑zero).              */
    uint32_t *sq = (uint32_t *)malloc(sq_cnt * sizeof(uint32_t));
    if (!sq) return;
    for (size_t i = 0; i < sq_cnt; ++i) sq[i] = (uint32_t)((i + 1) * (i + 1));

    /* Allocate and zero the convolution array.           */
    size_t sum_sz = (size_t)new_max_sum + 1;
    uint32_t *conv = (uint32_t *)calloc(sum_sz, sizeof(uint32_t));
    if (!conv) { free(sq); return; }

    /* Count all ordered triples of squares.               */
    for (size_t i = 0; i < sq_cnt; ++i) {
        for (size_t j = 0; j < sq_cnt; ++j) {
            uint32_t sum2 = sq[i] + sq[j];
            if (sum2 > new_max_sum) continue;
            for (size_t k = 0; k < sq_cnt; ++k) {
                uint32_t tot = sum2 + sq[k];
                if (tot > new_max_sum) break;
                ++conv[tot];
            }
        }
    }

    /* Count triples with all three values equal.          */
    uint8_t *all_equal = (uint8_t *)calloc(sum_sz, sizeof(uint8_t));
    if (!all_equal) { free(sq); free(conv); return; }
    for (size_t i = 0; i < sq_cnt; ++i) {
        uint32_t r = sq[i];
        uint64_t n = (uint64_t)r * 3ULL;
        if (n <= new_max_sum) all_equal[(size_t)n] = 1;
    }

    /* Count triples with exactly two equal values.        */
    uint16_t *dup_two = (uint16_t *)calloc(sum_sz, sizeof(uint16_t));
    if (!dup_two) { free(sq); free(conv); free(all_equal); return; }
    for (size_t ri = 0; ri < sq_cnt; ++ri) {
        uint32_t r = sq[ri];
        for (size_t di = 0; di < sq_cnt; ++di) {
            if (di == ri) continue;
            uint64_t n = (uint64_t)r * 2ULL + (uint32_t)sq[di];
            if (n <= new_max_sum) ++dup_two[(size_t)n];
        }
    }

    /* Build the sorted list of qualifying numbers.       */
    size_t new_cap = 1024;
    seq_t *new_list = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_list) { free(sq); free(conv); free(all_equal); free(dup_two); return; }
    size_t new_len = 0;

    for (idx_t val = 1; val <= new_max_sum; ++val) {
        uint32_t total       = conv[(size_t)val];
        if (total == 0) continue;

        uint32_t dup_all     = all_equal[(size_t)val];
        uint32_t dup_two_cnt = (uint32_t)dup_two[(size_t)val] * 3U;
        uint32_t ordered_distinct = total - dup_all - dup_two_cnt;

        /* 6 permutations per unordered distinct triple. */
        if (ordered_distinct >= 54U) {
            if (new_len == new_cap) {
                new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(new_list, new_cap * sizeof(seq_t));
                if (!tmp) { free(new_list); break; }
                new_list = tmp;
            }
            new_list[new_len++] = (seq_t)val;
        }
    }

    /* Free temporaries.                                      */
    free(sq);
    free(conv);
    free(all_equal);
    free(dup_two);

    /* Replace global list.                                    */
    seq_t *old = seq_list;
    seq_list = new_list;
    seq_len  = new_len;
    if (old) free(old);

    current_max_sum = new_max_sum;
}

/* --------------------------------------------------------------------- */
/* Ensure the array contains at least n terms.                           */
static void ensure_terms(idx_t n)
{
    if (current_max_sum == 0) recompute(32768);  /* initial limit  */
    while (seq_len < (size_t)n) {
        recompute(current_max_sum * 2ULL);
    }
}

/* --------------------------------------------------------------------- */
/* Exported function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant  */
    ensure_terms(n);
    return seq_list[(size_t)n - 1];
}
