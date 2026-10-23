/*
 * A100387  a(n) is the largest number x such that for m=n to n+x-1,
 *          A006530(m) decreases.
 * https://oeis.org/A100387
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the length of the longest initial run of strictly
 *            decreasing largest prime factors of consecutive integers,
 *            starting at n. Computed by iterating from n upward, comparing
 *            successive largest prime factors via greatest_prime_factor
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   greatest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/greatest_prime_factor.h"

seq_t a(idx_t n) {
    /* offsets: the sequence only defined for n >= 2,
       but we simply return 0 for smaller indices. */
    if (n < 2) return 0;

    seq_t curr = greatest_prime_factor((seq_t)n);
    seq_t count = 1;          /* include n itself */
    idx_t m = n + 1;

    while (1) {
        seq_t next = greatest_prime_factor((seq_t)m);
        if (next >= curr) break;   /* not decreasing any more */
        curr = next;
        count++;
        ++m;
    }

    return count;
}
