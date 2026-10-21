/*
 * A059843  a(n) is the smallest prime p such that p-n is a nonzero square.
 * https://oeis.org/A059843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ p prime : p – n is a non‑zero perfect square }
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1; n==0 is undefined but return 0 */
        return 0;

    const seq_t base = (seq_t)n;

    for (idx_t x = 1; ; ++x) {
        const seq_t sq  = (seq_t)x * (seq_t)x;
        const seq_t p   = base + sq;

        if (is_prime_big(p))
            return p;
    }
}
