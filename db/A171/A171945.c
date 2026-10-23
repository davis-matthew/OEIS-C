/*
 * A171945  P-positions for game of misere version of Mark.
 * https://oeis.org/A171945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1. For n > 1, a(n) is the n‑th even integer such that
 *            either (i) the exponent of 2 in its factorisation (the number of
 *            trailing zeros in binary) is odd, or (ii) it is a power of 2
 *            whose exponent is even
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This definition matches the OEIS entry A171945. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq = NULL;          /* cached sequence values   */
static size_t seq_len = 0;         /* number of values cached  */
static size_t seq_cap = 0;         /* allocation capacity     */
static idx_t next_candidate = 2;   /* next integer to test    */

/* Helper: test if x is a power of 2 (x>0). */
static inline bool is_pow2(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Helper: compute the exponent of 2 dividing x
   (i.e. count trailing zeros).  */
static inline int trailing_zeros(idx_t x)
{
    int tz = 0;
    while ((x & 1) == 0) {
        ++tz;
        x >>= 1;
    }
    return tz;
}

/* Ensure seq has space for at least 'need' elements. */
static inline void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!p) {
        /* Allocation failure – abort with a sensible error.  */
        abort();
    }
    seq = p;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for offsets >=1.  */
        abort();
    }
    /* Ensure the first term (1) is cached. */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        next_candidate = 2;      /* start searching from 2 */
    }
    /* If the requested term is already cached, return it. */
    if ((idx_t)seq_len >= n) {
        return seq[n - 1];
    }

    /* Generate terms until we have at least n terms cached. */
    while (seq_len < n) {
        /* Find the next candidate that satisfies the condition. */
        while (true) {
            /* Skip odd numbers. */
            if ((next_candidate & 1) != 0) {
                ++next_candidate;
                continue;
            }
            int tz = trailing_zeros(next_candidate);
            bool pow2 = is_pow2(next_candidate);
            if ((pow2 && ((tz & 1) == 0)) || (!pow2 && ((tz & 1) == 1))) {
                /* this number belongs to the sequence */
                break;
            }
            ++next_candidate;
        }

        /* Store the found number. */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = (seq_t)next_candidate;
        ++seq_len;
        ++next_candidate;        /* advance to the next integer for the next search */
    }
    return seq[n - 1];
}
