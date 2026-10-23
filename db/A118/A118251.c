/*
 * A118251  a(1)=1. Concatenate onto the end of the sequence (from left to
 *          right) the integer m_n converted into binary and reversed (with
 *          the most significant digit on the right), where m_n is the
 *          smallest integer > A118252(n-1) and whose reversed binary
 *          representation does not occur anywhere earlier in the...
 * https://oeis.org/A118251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   This function generates the binary sequence A118251. It appends,
 *            for each new term, * the reversed binary representation of the
 *            smallest integer m greater than the previous * m that has not
 *            occurred as a contiguous substring in the sequence constructed
 *            so far. * The function returns the bit at po...
 * Range:     u8 n <= 9381, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   pattern_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pattern_exists.h"

static uint8_t *seq_bits = NULL;   /* dynamic array holding the sequence bits */
static size_t seq_len  = 0;         /* current number of bits in seq_bits */
static size_t seq_cap  = 0;         /* allocated capacity of seq_bits */
static uint64_t current_m = 0;      /* last m used in the construction */

/* Increase the capacity of seq_bits to at least 'needed' elements. */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    uint8_t *new_buf = (uint8_t *)realloc(seq_bits, new_cap);
    if (!new_buf)
        abort();                  /* abort on allocation failure */
    seq_bits = new_buf;
    seq_cap  = new_cap;
}

/* External function required by the build: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure the sequence is long enough to contain n terms. */
    while (seq_len < (size_t)n) {
        /* Find the next m: smallest integer > current_m whose reversed binary
         * representation does not occur in the sequence so far. */
        uint64_t candidate = current_m + 1;
        uint8_t pat[128];
        size_t pat_len;

        while (true) {
            /* Build reversed binary representation of 'candidate'. */
            uint64_t temp = candidate;
            pat_len = 0;
            while (temp > 0) {
                pat[pat_len++] = (uint8_t)(temp & 1U);
                temp >>= 1;
            }
            if (pat_len == 0) {          /* should not happen for positive candidate */
                pat[pat_len++] = 0;
            }

            /* Check if this pattern appears in the existing sequence. */
            if (!pattern_exists(seq_bits, seq_len, pat, pat_len))
                break;                  /* candidate accepted */

            ++candidate;                 /* try next integer */
        }

        /* Append the accepted pattern to the sequence. */
        grow_seq(seq_len + pat_len);
        memcpy(seq_bits + seq_len, pat, pat_len);
        seq_len = seq_len + pat_len;
        current_m = candidate;
    }

    /* Return the required bit, cast to seq_t. */
    return (seq_t)seq_bits[(size_t)n - 1];
}
