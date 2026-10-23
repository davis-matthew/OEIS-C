/*
 * A200956  Primes of the form 8*k^3-3.
 * https://oeis.org/A200956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search over k to output primes of the form 8*k^3-3. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        seq_t k_seq = (seq_t)k;
        seq_t x = k_seq * k_seq;   /* k^2 */
        x *= k_seq;                /* k^3 */
        x *= 8;                    /* 8*k^3 */
        x -= 3;                    /* 8*k^3-3 */
        if (is_prime_seq(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++k;
    }
    return 0; /* n is always >= 1, so this line is never reached */
}
