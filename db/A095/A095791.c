/*
 * A095791  Number of digits in lazy-Fibonacci-binary representation of n
 *          (A104326).
 * https://oeis.org/A095791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(floor(n/phi)) + 1 (phi = (1+√5)/2)
 *            The function computes a(n) by iteratively applying floor(n/phi)
 *            using Fibonacci approximations
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_div_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_div_phi.h"

/* ------------ sequence function -------------------------------- */
seq_t a(idx_t n) {
    /* base cases */
    if (n == 0 || n == 1)
        return 1;

    uint64_t cur = (uint64_t)n;
    uint64_t cnt = 0;
    while (cur >= 2) {
        cur = floor_div_phi(cur);
        ++cnt;
    }
    return (seq_t)(cnt + 1);
}
