/*
 * A060251  a(n) = periodic part of decimal expansion of n/n-th prime (leading
 *          0's moved to end).
 * https://oeis.org/A060251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = periodic part of the decimal expansion of n/p_n * (p_n =
 *            n-th prime, leading zeros moved to the end)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Get the n-th prime */
    seq_t sp = get_prime(n);
    uint64_t p = (uint64_t)sp;          /* p <= 2^32 for n <= 10000 */

    /* For primes 2 and 5 the decimal expansion terminates,
     * so the periodic part is 0. */
    if (p == 2 || p == 5) {
        return 0;
    }

    /* Compute the order of 10 modulo p (length of the repeating block). */
    uint64_t order = 1;
    uint64_t t = 10 % p;
    while (t != 1) {
        t = (t * 10) % p;
        ++order;
    }

    /* Generate the digits of the repeating block, starting from remainder r = n */
    uint64_t r = n % p;          /* n < p for all n >= 1 */
    seq_t val = 0;               /* integer formed from digits after leading zeros */
    uint64_t leading_zero_cnt = 0;
    int started = 0;             /* flag: whether first non‑zero digit has appeared */

    for (uint64_t i = 0; i < order; ++i) {
        r *= 10;
        uint64_t digit = r / p;
        r %= p;

        if (!started) {
            if (digit == 0) {
                ++leading_zero_cnt;
            } else {
                started = 1;
                val = digit;                            /* digit is small, fits in seq_t */
            }
        } else {
            val = val * 10 + digit;
        }
    }

    /* Shift the leading zeros to the end: multiply by 10^leading_zero_cnt */
    for (uint64_t i = 0; i < leading_zero_cnt; ++i) {
        val = val * 10;
    }

    return val;
}
