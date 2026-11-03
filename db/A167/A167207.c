/*
 * A167207  Numbers that are not divisible by a smaller number that is a
 *          square greater than 1.
 * https://oeis.org/A167207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9713 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A167207 - numbers that are not divisible by a smaller square >1 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether a number is valid: it must not be divisible by any perfect
 * square greater than 1 and smaller than itself.  Perfect squares themselves
 * are considered valid. */
static bool term_is_valid(seq_t x)
{
    idx_t i = 2;
    for (; (seq_t)i * (seq_t)i <= x; ++i)
    {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (x % sq == 0)
            return sq == x;   /* valid only if the square equals the number itself */
    }
    return true;
}

/* maximum number of terms we precompute – sufficient for n up to very large values */
#define MAX_TERMS 200000

static seq_t terms[MAX_TERMS];
static size_t terms_size = 0;
static seq_t next_candidate = 1;

/* External function providing the n-th term of the sequence.
 * The function remembers previously computed terms so that any
 * sequence of calls is efficient. */
seq_t a(idx_t n)
{
    /* Ensure that we have at least n terms precomputed. */
    while (terms_size < (size_t)n)
    {
        seq_t val = next_candidate;
        if (term_is_valid(val))
            terms[terms_size++] = val;
        ++next_candidate;
    }
    return terms[n - 1];
}
