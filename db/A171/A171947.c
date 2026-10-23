/*
 * A171947  P-positions for game of UpMark.
 * https://oeis.org/A171947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A171947 – P‑positions for the game of UpMark
 *
 *  The sequence consists of all odd positive integers x such that the
 *  exponent of 2 in the factorisation of x−1 is odd (i.e. v₂(x−1) is odd).
 *  Equivalently, x ≡ 2^k+1 (mod 2^(k+1)) for some odd k ≥ 1.
 *  A short test:   x−1 = 2^k·m with k odd and m odd.
 *
 *  The function memoises the terms, so calls in any order are fast.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    /*  Handle index < 1 (although offset is 1); return 0 as a safe value. */
    if (n < 1) return 0;

    /*  Static cache of the sequence:  seq[0] = 1, seq[1] = 3, ...  */
    static seq_t *seq = NULL;
    static size_t size = 0;      /* number of terms stored */
    static size_t capacity = 0;  /* allocated capacity */
    static idx_t next_odd = 3;   /* next odd number to test */

    /*  Initialise the cache on the first call.  */
    if (seq == NULL) {
        capacity = 128;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq) return 0;           /* allocation failure – unlikely */
        seq[0] = 1;                   /* a(1) = 1 */
        size = 1;
    }

    /*  Generate terms until we have at least n of them.  */
    while (size < n) {
        /*  Search for the next odd number with odd v₂(exponent) of x−1.  */
        while (1) {
            /*  Compute the exponent of 2 in (next_odd−1).  */
            idx_t v = next_odd - 1;
            unsigned int exp = 0;
            while ((v & 1) == 0) {
                v >>= 1;
                exp++;
            }
            /*  Accept if exp is odd.  */
            if ((exp & 1) == 1)
                break;                /* found a valid term */
            next_odd += 2;            /* try next odd candidate */
        }

        /*  Store the found term.  */
        if (size == capacity) {        /* grow the buffer if needed */
            capacity *= 2;
            seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
            if (!seq) return 0;        /* allocation failure */
        }
        seq[size] = (seq_t)next_odd;
        size++;

        /*  Prepare candidate for the next iteration.  */
        next_odd += 2;
    }

    return seq[n - 1];
}
