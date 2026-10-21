/*
 * A069120  Integers of the form k!*sum(i=1..k, 1/(i*2^i) ).
 * https://oeis.org/A069120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * sum(i=1..n, 1/(i*2^i)), offset 1 This implementation
 *            generates the sequence elements incrementally and memoises them
 *            for fast repeated calls
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   div_by_pow2, v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t if needed */
#include "../utils/div_by_pow2.h"
#include "../utils/v2.h"

extern seq_t a(idx_t n);   /* forward declaration for compiler/tooling */

/* Static storage for memoised terms (up to 10000 elements as requested). */
static seq_t seq_vals[10000];
static idx_t seq_len = 0;      /* number of terms stored */

/* State maintained between successive calls. */
static idx_t next_k = 1;        /* next k to process (starts from 1) */
static seq_t fact = 1;          /* (next_k-1)!  (starts with 0! = 1) */
static seq_t S   = 0;           /* S_{next_k-1}  (starts with S_0 = 0) */

seq_t a(idx_t n) {
    /* Ensure we have generated enough terms. */
    while (seq_len < n && seq_len < 10000) {
        /* Update factorial: (next_k)! = fact * next_k. */
        seq_t fact_prev = fact;
        fact *= next_k;

        /* Update sum S_k = 2*next_k*S_{k-1} + (k-1)! . */
        seq_t S_prev = S;
        S = (seq_t)2 * next_k * S_prev + fact_prev;

        /* Check integer condition: v2(S) >= next_k. */
        if (v2(S) >= next_k) {
            /* Compute a_k = S / 2^next_k. */
            seq_t a_k = div_by_pow2(S, next_k);
            seq_vals[seq_len++] = a_k;
        }

        ++next_k;   /* prepare for the next k */
    }

    /* Return the requested term (1-based index). */
    return seq_vals[n - 1];
}
