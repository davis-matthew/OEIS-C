/*
 * A163751  a(n) = n-th even nonprime minus n-th nonprime.
 * https://oeis.org/A163751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 334, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth even nonprime (starting at 0) minus nth nonprime (starting at 0). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t even_np;
    if (n == 1) {
        even_np = (seq_t)0;
    } else {
        even_np = (seq_t)2 * (seq_t)n;   // 2,4,6,... starting at n=2
    }

    idx_t count = 0;
    idx_t value = 0;
    while (1) {
        /* n-th nonprime: 0 and 1 are nonprime, others where is_prime == false */
        if (value < 2 || !is_prime(value)) {
            count++;
            if (count == n) break;
        }
        value++;
    }

    seq_t non_np = (seq_t)value;
    return even_np - non_np;
}
