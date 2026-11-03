/*
 * A061806  Numbers n such that the iterative cycle: n -> sum of digits of n^2
 *          has only three distinct elements.
 * https://oeis.org/A061806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   digit_frequencies, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A061806: numbers n such that the iterative cycle
 * n -> sum of digits of n^2 has exactly three distinct elements.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_frequencies.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}
#include <stdbool.h>

static seq_t digit_sum_square(seq_t x)
{
    seq_t sq = seq_mul(x, x);
    unsigned int freq[10] = {0};
    digit_frequencies(sq, freq);
    seq_t sum = 0;
    for (int d = 0; d < 10; ++d)
        sum += (seq_t)d * freq[d];
    return sum;
}

static bool qualifies(seq_t n)
{
    seq_t seen[3];
    size_t seen_len = 0;
    seq_t curr = n;
    while (1) {
        seq_t next = digit_sum_square(curr);
        for (size_t i = 0; i < seen_len; ++i) {
            if (seen[i] == next) {
                return seen_len == 3;
            }
        }
        if (seen_len >= 3)
            return false;
        seen[seen_len++] = next;
        curr = next;
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t last_tested = 0;

    if (n == 0)
        return 0;   /* Undefined for offset 0, but safe guard */

    while (terms_len < n) {
        last_tested += 1;
        if (qualifies(last_tested)) {
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = last_tested;
        }
    }
    return terms[n - 1];
}
