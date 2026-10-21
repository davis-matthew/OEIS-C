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
 * Helpers:   digit_frequencies, grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A061806: numbers n such that the iterative cycle
 * n -> sum of digits of n^2 has exactly three distinct elements.
 */

#include "../utils/seq_mul.h"
#include "../utils/digit_frequencies.h"
#include "../utils/grow_seq.h"
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
