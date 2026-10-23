/*
 * A190664  Least semiprime whose prime factors differ by 2*prime(n).
 * https://oeis.org/A190664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime whose prime factors differ by 2*prime(n)
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   get_prime, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t d = (seq_t)2 * (seq_t)get_prime(n);
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime_big((seq_t)p))
            continue;
        seq_t q = (seq_t)p + d;
        if (!is_prime_big(q))
            continue;
        return seq_mul((seq_t)p, q);
    }
}
