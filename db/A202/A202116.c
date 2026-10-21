/*
 * A202116  Numbers k such that 90*k + 89 is prime.
 * https://oeis.org/A202116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k (a(1)=0) such that 90*k+89 is prime
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of bounds – return 0 */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        seq_t cand = (seq_t)90 * (seq_t)k + (seq_t)89;
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
