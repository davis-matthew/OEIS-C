/*
 * A005765  Size of Doehlert-Klee design with n blocks.
 * https://oeis.org/A005765
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 if n is squarefree; otherwise a(n) = n - pi(p), where p
 *            is the smallest prime with p^2 | n and pi is the prime-counting
 *            function
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   small_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/small_is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t d = 2; d * d <= n; d++) {
        if (!small_is_prime(d)) continue;
        count++;
        if (n % (d * d) == 0)
            return (seq_t)n - (seq_t)count;
    }
    return (seq_t)1;
}
