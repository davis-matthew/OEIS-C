/*
 * A118754  Smallest prime >= 5*n.
 * https://oeis.org/A118754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime >= 5*n
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    // Calculate 5*n, ensuring seq_t arithmetic
    seq_t target = (seq_t)5 * (seq_t)n;

    // Start candidate at target; handle n = 0 case where target < 2
    seq_t candidate = target;
    if (candidate < (seq_t)2) candidate = (seq_t)2;

    // Candidate 2 is the only even prime
    if (candidate == (seq_t)2) return (seq_t)2;

    // Make sure candidate is odd (skip even numbers > 2)
    if ((candidate & (seq_t)1) == (seq_t)0) candidate += (seq_t)1;

    // Search for the next prime
    for (;;) {
        if (is_prime_small(candidate)) return candidate;
        candidate += (seq_t)2;   // skip even numbers
    }
}
