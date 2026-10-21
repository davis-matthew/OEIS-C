/*
 * A072969  Least k > 0 such that the last digit of k^n is the same as the
 *          last digit of n^k.
 * https://oeis.org/A072969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   modpow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Brute‑force search for the smallest k>0 such that k^n mod 10 == n^k mod 10 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/modpow10.h"

seq_t a(idx_t n)
{
    idx_t last_n = n % 10;
    for (idx_t k = 1; ; ++k) {
        if (modpow10(k, n) == modpow10(last_n, k))
            return (seq_t)k;
    }
}
