/*
 * A129540  Triangular numbers T such that T+10 is the next prime after T.
 * https://oeis.org/A129540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number T such that T+10 is the next
 *            prime after T. That is, T is triangular, T+10 is prime, and none
 *            of T+1,…,T+9 are prime
 * Range:     u8 none, u16 n <= 10, u32 n <= 12, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;                 /* index of the triangular number */

    for (;; ++m) {
        seq_t T = (seq_t)m * ((seq_t)m + 1) / 2;   /* <C> triangular number */
        seq_t T_plus10 = T + (seq_t)10;

        /* 1. T+10 must be prime */
        if (!is_prime_big(T_plus10))
            continue;

        /* 2. No other prime between T+1 and T+9 */
        bool bad = false;
        for (uint8_t k = 1; k <= 9; ++k) {
            if (is_prime_big(T + (seq_t)k)) {
                bad = true;
                break;
            }
        }
        if (bad)
            continue;

        /* Found a valid triangular number */
        ++count;
        if (count == n)
            return T;
    }
}
