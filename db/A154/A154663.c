/*
 * A154663  Numbers k such that k! + 767 and abs(k! - 767) are primes.
 * https://oeis.org/A154663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k! + 767 and abs(k! - 767) are primes. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t) */

seq_t a(idx_t n) {
    /* Only k up to 12 need be tested: for k >= 13, k! is divisible by 13,
       hence k! + 767 is divisible by 13 and cannot be prime. */
    idx_t wanted = n;
    idx_t count = 0;

    for (idx_t k = 1; k <= 12; ++k) {
        /* Compute k! as seq_t. */
        seq_t fact = 1;
        for (idx_t i = 2; i <= k; ++i) fact *= i;

        /* k! + 767 */
        seq_t sum = fact + (seq_t)767;

        /* |k! - 767| */
        seq_t diff = (fact > (seq_t)767) ? fact - (seq_t)767
                                         : (seq_t)767 - fact;

        if (is_prime_big(sum) && is_prime_big(diff)) {
            ++count;
            if (count == wanted)
                return (seq_t)k;
        }
    }

    /* Sequence terminates; return 0 for n beyond known terms. */
    return (seq_t)0;
}
