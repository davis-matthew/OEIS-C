/*
 * A004613  Numbers that are divisible only by primes congruent to 1 mod 4.
 * https://oeis.org/A004613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Numbers divisible only by primes ≡ 1 (mod 4), by trial division
 * Range:     u8 n <= 34, u16 n <= 6420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_only_primes_1mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_only_primes_1mod4.h"

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < target) {
        num++;
        if (has_only_primes_1mod4(num)) {
            count++;
        }
    }
    
    return num;
}
