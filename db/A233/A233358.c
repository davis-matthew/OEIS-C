/*
 * A233358  Numbers n such that n^2 concatenated with n + 1 is prime.
 * https://oeis.org/A233358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the nth even integer n for which the decimal
 *            concatenation of n^2 and (n+1) is prime. This implementation
 *            searches sequentially through even integers and memoizes found
 *            terms so that any call to a(n) will return the correct value in
 *            O(1) average time after the first few terms have been co...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 643, u64 n <= 5550,
 *            u128 n <= 5550, big any n
 * Helpers:   append_candidate, digits_of, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/digits_of.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of found terms */
    static size_t seq_len = 0;  /* number of terms stored */
    static size_t seq_cap = 0;  /* current capacity of seq */
    static idx_t next_candidate = 2; /* next even integer to test */

    /* Ensure the results array is initialized */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0; /* should not happen */
    }

    /* Generate terms until we have at least n terms */
    while (seq_len < n) {
        /* Concatenate n^2 with n+1 */
        seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;
        int digs = digits_of((idx_t)(next_candidate + 1));
        seq_t concat = sq * pow10((idx_t)digs) + (seq_t)(next_candidate + 1);

        if (isPrimeGeneric(concat)) {
            /* Append to the stored sequence, resizing if necessary */
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap * 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = next_candidate;
        }

        next_candidate += 2; /* only even candidates are possible */
    }

    return seq[n - 1];
}
