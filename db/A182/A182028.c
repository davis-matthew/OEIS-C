/*
 * A182028  Take first n bits of the infinite Fibonacci word A003849, regard
 *          them as a binary number, then convert it to base 10.
 * https://oeis.org/A182028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*a(n-1) + A003849(n) (a(0) = 0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the Fibonacci word incrementally,
   stores the first N+1 bits in a static buffer, and computes the
   binary number for any requested n.  It works for any unsigned
   integer seq_t (including arbitrary-precision) and for idx_t = uint64_t. */

#include <stdlib.h>
#include <string.h>

/* Return the n-th term of OEIS A182028 */
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0U)
        return 0U;

    /* static buffer to cache Fibonacci word bits for reuse */
    static unsigned char *bits = NULL;
    static size_t bits_len = 0; /* length of data stored in bits */

    /* need the first n+1 bits */
    size_t need = (size_t)n + 1U;

    /* rebuild the buffer if we don't have enough bits yet */
    if (bits_len < need) {
        /* free old buffer if present */
        if (bits) {
            free(bits);
            bits = NULL;
        }
        /* build the word until it is long enough */
        unsigned char *prev = (unsigned char *)malloc(1U);
        if (!prev) return 0U;
        prev[0] = 0U;
        size_t prev_len = 1U;

        unsigned char *curr = (unsigned char *)malloc(2U);
        if (!curr) { free(prev); return 0U; }
        curr[0] = 0U;
        curr[1] = 1U;
        size_t curr_len = 2U;

        while (curr_len < need) {
            size_t next_len = (size_t)curr_len + (size_t)prev_len;
            unsigned char *next = (unsigned char *)malloc(next_len);
            if (!next) {
                free(prev);
                free(curr);
                return 0U;
            }
            memcpy(next, curr, curr_len);
            memcpy(next + curr_len, prev, prev_len);
            free(prev);
            prev = curr;
            prev_len = curr_len;
            curr = next;
            curr_len = next_len;
        }

        /* store the first 'need' bits in the static buffer */
        bits = (unsigned char *)malloc(need);
        if (!bits) {
            free(prev);
            free(curr);
            return 0U;
        }
        memcpy(bits, curr, need);
        bits_len = need;

        free(prev);
        free(curr);
    }

    /* interpret bits[0..n] as a binary number */
    seq_t value = 0U;
    for (size_t i = 0U; i < need; ++i) {
        value = (value << 1) + (seq_t)bits[i];
    }
    return value;
}
