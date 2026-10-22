/*
 * A002340  Numbers x such that p = x^2 - 5y^2, where p == 0, 1, or 4 (mod 5).
 * https://oeis.org/A002340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for the n-th prime p where p ≡ 0, 1, or 4 (mod 5), find the
 *            minimal y ≥ 0 such that p + 5y^2 is a perfect square, return the
 *            corresponding x value
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isprime.h"

static seq_t isqrt_seq(seq_t x) {
    if (x == 0) return 0;
    seq_t s = x;
    for (int i = 0; i < 100; i++) {
        seq_t r = (s + x / s) / 2;
        if (r >= s) return s;
        s = r;
    }
    return s;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t p = 2;
    
    while (count < (uint64_t)n) {
        if (isprime(p)) {
            int mod5 = p % 5;
            if (mod5 == 0 || mod5 == 1 || mod5 == 4) {
                count++;
                if (count == (uint64_t)n) {
                    seq_t p_seq = (seq_t)p;
                    seq_t max_y = 2 * isqrt_seq(p_seq) + 100;
                    
                    for (seq_t y = 0; y <= max_y; y++) {
                        seq_t val = p_seq + 5 * y * y;
                        seq_t x = isqrt_seq(val);
                        if (x * x == val) {
                            return x;
                        }
                    }
                    return 0;
                }
            }
        }
        p = (p == 2) ? 3 : p + 2;
    }
    
    return 0;
}
