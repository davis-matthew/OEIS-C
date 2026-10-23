/*
 * A129287  Number of Harshad numbers between 10^n and 10^n + 100.
 * https://oeis.org/A129287
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # { k : 0≤k≤100 , (10^n + k) mod (1 + digit_sum(k)) == 0
 *            }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum_small.h"

/* Compute 10^exp mod mod (mod ≤ 19).  exp may be up to 10^4. */
static idx_t pow10_mod(idx_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    idx_t base = 10 % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* a(n) counts Harshad numbers between 10^n and 10^n+100 inclusive. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* behaviour for n < offset is irrelevant */

    seq_t count = 0;
    for (idx_t k = 0; k <= 100; ++k) {
        idx_t d = 1 + digit_sum_small(k);          /* divisor, 1 ≤ d ≤ 19 */
        idx_t mod10n = pow10_mod(n, d);             /* 10^n mod d */
        idx_t m = (mod10n + k % d) % d;             /* (10^n + k) mod d */
        if (m == 0)
            ++count;
    }
    return count;
}
