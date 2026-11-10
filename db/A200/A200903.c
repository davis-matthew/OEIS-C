/*
 * A200903  A182589(n)/8.
 * https://oeis.org/A200903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(p-2)+1)*(2^(p-1)-1)/p where p = prime(n); a(1)=1
 *            (this is A182589(n)/8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

static seq_t A200903_pw2(uint64_t e) {
    seq_t r = 1;
    for (uint64_t i = 0; i < e; i++) r = r * 2;
    return r;
}

seq_t a(idx_t n) {
    uint64_t p = nth_prime(n);
    if (p == 2) return 1;
    /* for odd p, p divides 2^(p-1)-1 (Fermat), so divide first to avoid overflow */
    seq_t part2 = (A200903_pw2(p - 1) - 1) / (seq_t)p;
    seq_t part1 = A200903_pw2(p - 2) + 1;
    return part1 * part2;
}
