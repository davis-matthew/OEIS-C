/*
 * A201056  Composite numbers whose product of digits is 8.
 * https://oeis.org/A201056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute by generating composite numbers whose product of digits
 *            equals 8, using a recursive search for each digit length
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/seq_cmp.h"

static const int allowed_digits[] = {1, 2, 4, 8};
static const size_t NUM_DIGITS = sizeof(allowed_digits) / sizeof(allowed_digits[0]);

/* Recursive helper: build all numbers of a given length that satisfy
   the remaining digit product (remProd). */
static void gen_rec(int len, int pos, int remProd, seq_t cur,
                    seq_t **seq, size_t *len_ptr, size_t *cap_ptr)
{
    if (pos == len) {
        if (remProd == 1) {
            if (!is_prime_big(cur))
                append_candidate(seq, len_ptr, cap_ptr, cur);
        }
        return;
    }

    /* If remaining product is 1, all remaining positions must be 1's. */
    if (remProd == 1) {
        seq_t val = cur;
        for (int i = pos; i < len; ++i)
            val = val * 10 + 1;
        if (!is_prime_big(val))
            append_candidate(seq, len_ptr, cap_ptr, val);
        return;
    }

    for (size_t i = 0; i < NUM_DIGITS; ++i) {
        int d = allowed_digits[i];
        if (remProd % d != 0)
            continue;
        seq_t next = cur * 10 + (seq_t)d;
        gen_rec(len, pos + 1, remProd / d, next, seq, len_ptr, cap_ptr);
    }
}

/* Generate all numbers of a specific length and append the composites. */
static void generate_length(int len, seq_t **seq, size_t *len_ptr, size_t *cap_ptr)
{
    gen_rec(len, 0, 8, 0, seq, len_ptr, cap_ptr);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static int max_len_generated = 0;

    /* Generate numbers until we have at least n terms. */
    while (seq_len < n) {
        ++max_len_generated;
        generate_length(max_len_generated, &seq, &seq_len, &seq_cap);
    }

    /* Sort the sequence once per augmentation. */
    qsort(seq, seq_len, sizeof(seq_t),
          (int(*)(const void *, const void *))seq_cmp);

    return seq[n - 1];
}
