/*
 * A179953  a(n) is the least exponent k such that q^k >= n, where q is the
 *          greatest prime factor of n (= A006530(n)); a(1) = 1 by convention.
 * https://oeis.org/A179953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 308, u16 n <= 9459, u32 n <= 9459, u64 n <= 9459,
 *            u128 n <= 9459, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the smallest exponent k with (largest prime factor of n)^k ≥ n. */
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;                  /* convention */
    uint64_t q = largest_prime_factor((uint64_t)n);  /* greatest prime factor of n */

    seq_t pow = 1;                          /* current power q^k */
    seq_t k   = 0;                           /* exponent counter */

    while (pow < (seq_t)n) {
        /* If next multiplication would reach or exceed n, finish the loop */
        if (pow > (seq_t)n / (seq_t)q) {
            k++;                             /* one more step is enough */
            break;
        }
        pow *= (seq_t)q;
        k++;
    }
    return k;
}
