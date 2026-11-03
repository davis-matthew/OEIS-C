/*
 * A063952  Of course every number is the sum of 4 squares; for these numbers
 *          the first square can be taken to be any square < n.
 * https://oeis.org/A063952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1105, u32 n <= 1105, u64 n <= 1105,
 *            u128 n <= 1105, big any n
 * Validated: standard, pass, 80 terms through n = 1061 (2026-11-03)
 * Helpers:   isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements sequence A063952.
    a(n) is the n‑th non‑negative integer b such that for every square
    t² < b, the number b−t² is not of the form 4^k(8m+7) (i.e. can be
    expressed as a sum of three squares).  The implementation tests each
    integer for this property, caching found terms so that calls are
    O(1) after the first discovery. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

static bool _three_sq_ok(seq_t n)
{
    if (n == 0) return true;
    /* remove all factors 4 */
    while ((n & 3) == 0) n >>= 2;
    /* n is of form 8k+7 -> not representable as a sum of three squares */
    return (n & 7) != 7;
}

/* check whether n satisfies the required property */
static bool _satisfies(seq_t n)
{
    if (n == 0) return true;
    seq_t tmax = isqrt_seq(n - 1);
    for (seq_t t = 0; t <= tmax; ++t) {
        seq_t rem = n - t * t;
        if (!_three_sq_ok(rem))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 1;

    /* initialise */
    if (terms_len == 0) {
        terms_cap = 64;
        terms = mem_new(seq_t, terms_cap);
        terms[0] = (seq_t)0; /* a(1) = 0 */
        terms_len = 1;
        next_candidate = 1;
    }

    while (terms_len < (size_t)n) {
        if (_satisfies(next_candidate)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}
