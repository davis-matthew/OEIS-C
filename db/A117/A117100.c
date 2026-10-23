/*
 * A117100  Numbers k such that nextprime(3*k) > 3*nextprime(k) and k is
 *          composite (if p is prime then nextprime(p) = p).
 * https://oeis.org/A117100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th composite integer k with the property
   nextprime(3*k) > 3*nextprime(k)  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* only for safety; offset starts at 1 */
    idx_t count = 0;
    uint64_t k = 4;  /* first composite number to test */
    while (true) {
        if (!is_prime(k)) {
            uint64_t p = next_prime(k);
            uint64_t q = next_prime(3 * k);
            if (q > 3 * p) {
                count++;
                if (count == n) {
                    return (seq_t)k;
                }
            }
        }
        k++;
    }
}
