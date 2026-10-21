/*
 * A092214  In base 2: numbers n not of the form k + reverse(k) for any k.
 * https://oeis.org/A092214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS A092214:
   numbers that are NOT of the form k + reverse(k) in binary.
   The implementation uses a simple sieve: for each k it marks
   k + reverse(k) as representable; numbers not marked become
   terms of the sequence.  The result is memoised so that
   calls with arbitrary indices are served quickly. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>        /* for uint8_t */

/* Reverse the binary digits of x (ignoring leading zeros). */
static idx_t reverse_bits(idx_t x)
{
    idx_t rev = 0;
    while (x) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Mark all sums k + reverse(k) for k <= upto.  Static data are
   passed by reference so that the function can modify them. */
static void ensure_sums_up_to(idx_t upto,
                              uint8_t **repr,
                              size_t   *repr_len,
                              idx_t    *k_next)
{
    while (*k_next <= upto) {
        idx_t rev  = reverse_bits(*k_next);
        idx_t sum  = *k_next + rev;

        /* Grow the representable array as required. */
        if (sum >= *repr_len) {
            size_t new_len = (*repr_len) ? (*repr_len) * 2 : 16;
            while (new_len <= sum)
                new_len <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(*repr, new_len);
            if (!tmp)
                abort();          /* out of memory */
            memset(tmp + *repr_len, 0, new_len - *repr_len);
            *repr     = tmp;
            *repr_len = new_len;
        }
        (*repr)[sum] = 1;
        (*k_next)++;
    }
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Memoised state. */
    static seq_t *seq      = NULL;
    static size_t seq_cap   = 0;
    static idx_t seq_len    = 0;

    static uint8_t *repr   = NULL;
    static size_t repr_len  = 0;

    static idx_t k_next     = 0;
    static idx_t candidate  = 0;

    while (seq_len <= n) {
        /* Ensure all representable sums up to the current candidate are known. */
        ensure_sums_up_to(candidate, &repr, &repr_len, &k_next);

        /* Ensure the representable array can be indexed by 'candidate'. */
        if (candidate >= repr_len) {
            size_t new_len = (repr_len) ? (repr_len * 2) : 16;
            while (new_len <= candidate)
                new_len <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(repr, new_len);
            if (!tmp)
                abort();          /* out of memory */
            memset(tmp + repr_len, 0, new_len - repr_len);
            repr      = tmp;
            repr_len  = new_len;
        }

        /* If candidate is not representable it is a term. */
        if (!repr[candidate]) {
            if (seq_len >= seq_cap) {
                size_t new_cap = (seq_cap) ? (seq_cap * 2) : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();      /* out of memory */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)candidate;
        }

        candidate++;
    }

    return seq[n];
}
