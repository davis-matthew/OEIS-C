/*
 * A161539  Numbers n such that composite (n) + n is a composite, where
 *          composite (n) denotes the n-th composite number.
 * https://oeis.org/A161539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number n such that composite(n) + n is composite
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    idx_t m = 0;
    idx_t comp_cnt = 0;
    seq_t cand = 4;        /* first composite number */
    seq_t comp_val = 0;    /* composite value for current m */

    while (found < n) {
        ++m; /* current index to test */

        /* generate the m-th composite number */
        while (comp_cnt < m) {
            while (is_prime_small(cand)) ++cand; /* skip primes */
            ++comp_cnt;
            comp_val = cand;
            ++cand; /* next candidate for future iterations */
        }

        seq_t sum = comp_val + (seq_t)m;
        if (!is_prime_small(sum)) {            /* sum is composite */
            ++found;
            if (found == n) return (seq_t)m;
        }
    }
    return (seq_t)m; /* unreachable, just to satisfy compiler */
}
