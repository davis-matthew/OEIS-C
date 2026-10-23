/*
 * A144411  Odd nonprime gaps adjusted to be {2,1,0,-1}: a(n)=A067970(n)/2-2.
 * https://oeis.org/A144411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = A067970(n)/2 - 2 where A067970(n) is the difference
 *            between the n‑th and (n‑1)‑th consecutive odd non‑prime numbers
 *            (odd composites, taking 1 as the first odd non‑prime)
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n) {
    if (n == 0) {
        /* The sequence is defined for n >= 1; return 0 for safety. */
        return (seq_t)0;
    }

    /* Generate odd non‑prime numbers until the n‑th gap is found. */
    idx_t last_nonprime = 1;   /* The first odd non‑prime. */
    idx_t candidate    = 3;   /* Next odd number to test. */
    idx_t idx          = 0;   /* Number of gaps found so far. */
    seq_t val          = 0;   /* Value of the current gap. */

    while (idx < n) {
        /* Find the next odd non‑prime. */
        while (is_prime(candidate)) {
            candidate += 2;
        }

        /* Compute the gap and the sequence value. */
        idx_t gap = candidate - last_nonprime;
        val = ((seq_t)gap / 2) - 2;

        /* Prepare for the next iteration. */
        last_nonprime = candidate;
        candidate += 2;
        idx++;
    }

    return val;
}
