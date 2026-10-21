/*
 * A093030  Largest palindromic divisor of n.
 * https://oeis.org/A093030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   compute a(n) as the largest palindromic divisor of n
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: enumerate divisors up to ⌊√n⌋, keeping the largest
   palindrome found.  The function uses only integer arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome_idx.h"

/* Integer square root: return ⌊√n⌋ for n >= 0.  Works for 64‑bit n. */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t sq = mid * mid;
        if (sq == n) {
            return mid;
        }
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }

    seq_t best = (seq_t)1;  /* 1 is always a palindromic divisor */
    idx_t limit = isqrt_u64(n);

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            if (is_palindrome_idx(d) && (seq_t)d > best) {
                best = (seq_t)d;
            }
            idx_t partner = n / d;
            if (partner != d && is_palindrome_idx(partner) && partner > (idx_t)best) {
                best = (seq_t)partner;
            }
        }
    }
    return best;
}
