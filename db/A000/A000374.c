/*
 * A000374  Number of cycles (mod n) under doubling map.
 * https://oeis.org/A000374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/ord(2,m))*Sum_{j = 0..ord(2,m)-1} gcd(2^j - 1, m),
 *            where m is n with all factors of 2 removed
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9005 (2026-11-01)
 * Helpers:   gcd_u64, odd_part_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_u64.h"
#include "../utils/odd_part_u64.h"

static uint64_t mult_order(uint64_t base, uint64_t m) {
    if (m == 1) return 1;
    uint64_t power = base % m;
    uint64_t ord = 1;
    while (power != 1) {
        power = (power * base) % m;
        ord++;
    }
    return ord;
}

seq_t a(idx_t n) {
    uint64_t m = odd_part_u64((uint64_t)n);
    uint64_t ord = mult_order(2, m);
    
    seq_t sum = 0;
    uint64_t power = 1;
    for (uint64_t j = 0; j < ord; j++) {
        uint64_t g = gcd_u64(power - 1, m);
        sum = sum + (seq_t)g;
        power = (power * 2) % m;
    }
    
    return sum / (seq_t)ord;
}
