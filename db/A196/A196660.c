/*
 * A196660  Smallest k>0 such that k*n+(n-1) is prime.
 * https://oeis.org/A196660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest k > 0 such that k*n + (n-1) is prime.
   Returns k as a seq_t value. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Offset starts at 1: a(1) = 2 */
    if (n == 1) return (seq_t)2;   /* k=2 gives 2 which is prime */

    /* Search k from 1 up to n-1 (conjectured bound) */
    for (idx_t k = 1; k < n; ++k) {
        seq_t val = (seq_t)k * (seq_t)n + (seq_t)(n - 1);
        if (is_prime_big(val)) return (seq_t)k;
    }

    /* Fallback: in case a prime was not found below n,
       continue searching beyond n until one is found.
       This is theoretically unnecessary but guarantees termination. */
    for (idx_t k = n; ; ++k) {
        seq_t val = (seq_t)k * (seq_t)n + (seq_t)(n - 1);
        if (is_prime_big(val)) return (seq_t)k;
    }
}
