/*
 * A108695  a(n) = number of times the digit congruent to n mod 10 has
 *          appeared in the sequence.
 * https://oeis.org/A108695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 650, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of times digit (n mod 10) has appeared in the sequence up to term n‑1 */
#include <stdlib.h>

static int digit_cnt[10] = {0};
static seq_t *seq = NULL;
static idx_t seq_len = 0;
static size_t seq_cap = 0;

static void add_digits(seq_t value) {
    if (value == 0) {
        ++digit_cnt[0];
        return;
    }
    while (value != 0) {
        int d = (int)(value % 10);
        ++digit_cnt[d];
        value /= 10;
    }
}

static void extend_sequence(idx_t n) {
    if (n + 1 <= seq_len)
        return;

    if (seq_cap < (size_t)(n + 1)) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 256;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    for (idx_t i = seq_len; i <= n; ++i) {
        int d = (int)(i % 10);
        seq[i] = (seq_t)digit_cnt[d];
        add_digits(seq[i]);
    }
    seq_len = n + 1;
}

seq_t a(idx_t n) {
    extend_sequence(n);
    return seq[n];
}
