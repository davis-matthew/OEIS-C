/*
 * A182231  Primes that are palindromic in base 32.
 * https://oeis.org/A182231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that is a palindrome in base 32
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_pal_ull, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_pal_ull.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0 */
    idx_t count = 0;
    idx_t L = 1;

    while (1) {
        int half_len = (int)((L + 1) / 2);
        uint64_t pow32 = 1;
        for (int i = 1; i < half_len; ++i) pow32 *= 32ULL;
        uint64_t start = pow32;
        uint64_t end   = pow32 * 32ULL - 1;
        if (end < start) break;    /* overflow, cannot generate further */

        for (uint64_t h = start; h <= end; ++h) {
            seq_t pal = build_pal_ull(h, (L % 2 == 1));
            if (pal == 0) continue;
            if (is_prime_big(pal)) {
                ++count;
                if (count == n) return pal;
            }
        }
        ++L;
    }
    return 0;  /* should not reach for valid n */
}
