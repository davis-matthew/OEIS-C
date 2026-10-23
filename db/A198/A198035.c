/*
 * A198035  Numbers n such that n^2 is a concatenation of two nonzero squares
 *          with no trailing zeros in n.
 * https://oeis.org/A198035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n ≥ 1 such that - n has no trailing zero,
 *            - n² can be split into two consecutive decimal blocks that are
 *            both perfect squares, - neither block may have leading zeros
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Test whether v is a perfect square (strictly >0). */
static bool is_square(seq_t v)
{
    if (v == 0) return false;
    seq_t r = floor_sqrt(v);
    return (r * r) == v;
}

/* Return true iff n satisfies the definition of the sequence. */
static bool is_valid_candidate(idx_t n)
{
    /* no trailing zeros */
    if ((n % 10) == 0) return false;

    seq_t sq = (seq_t)n * n;
    int len = digit_len(sq);
    if (len < 2) return false;          /* cannot split into two nonempty parts */

    for (int split = 1; split < len; ++split) {
        seq_t shift = pow10(len - split);          /* 10^(len-split) */
        seq_t left  = sq / shift;
        seq_t right = sq % shift;

        /* right part must have exactly (len-split) digits (no leading zeros) */
        if (right == 0) continue;
        if (right < pow10(len - split - 1)) continue;

        /* left part must have exactly split digits (no leading zeros) */
        if (left == 0) continue;
        if (left < pow10(split - 1)) continue;

        if (is_square(left) && is_square(right)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* On the first call allocate minimal storage; 8 is enough to begin. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t candidate = 1;

    while (terms_len < n) {
        if (is_valid_candidate(candidate)) {
            if (terms_cap == 0) {
                terms_cap = 8;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            } else if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return terms[n - 1];
}
