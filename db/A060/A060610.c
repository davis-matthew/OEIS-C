/*
 * A060610  Repeatedly apply Euler phi to the n-th prime; a(n) is the number
 *          of terms in the resulting iteration chain which are not powers of
 *          2 (number of initial iterations until reaching the first power of
 *          2).
 * https://oeis.org/A060610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of terms before the first power of 2 in the Euler
 *            phi‑iteration chain starting from the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 6915, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/calc_phi.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based). */
    seq_t p = (seq_t)get_prime(n);

    seq_t count = 0;
    seq_t cur = p;

    /* Count terms until the first power of 2 is reached. */
    while (!is_power_of_two(cur))
    {
        count++;
        cur = calc_phi(cur);
    }

    return count;
}
