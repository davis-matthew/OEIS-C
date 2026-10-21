/*
 * A069903  Number of distinct prime factors of n-th triangular number.
 * https://oeis.org/A069903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime factors of n(n+1)/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_distinct_primes.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is the index (n >= 1 by convention) */
    if (n == 0) return 0;  /* safety, not used */

    idx_t a_val = n;
    idx_t b_val = n + 1;
    /* divide one factor by 2 to obtain the triangular number */
    if ((n & 1) == 0) {          /* n even -> a_val /= 2 */
        a_val >>= 1;
    } else {                     /* n odd  -> b_val /= 2 */
        b_val >>= 1;
    }

    seq_t ca = _count_distinct_primes(a_val);
    seq_t cb = _count_distinct_primes(b_val);
    return ca + cb;
}
