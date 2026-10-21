/*
 * A085977  Number of 3's in decimal expansion of prime(n).
 * https://oeis.org/A085977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of digit '3's in the decimal expansion of the n‑th
 *            prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper in the utils library returns the n‑th prime. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n = 0 (outside the documented offset) return 0. */
    if (n == 0)
        return 0;

    /* Get the n-th prime; the n‑th prime for n≤10000 fits in uint32_t. */
    uint32_t p = get_prime(n);

    /* Count the decimal digits equal to 3. */
    seq_t count = 0;
    while (p > 0) {
        if (p % 10 == 3)
            ++count;
        p /= 10;
    }
    return count;
}
