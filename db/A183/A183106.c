/*
 * A183106  Numbers k such that sum of palindromic divisors of k (A088000(k))
 *          is palindromic.
 * https://oeis.org/A183106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the nth integer k such that the sum of its palindromic
   divisors is a palindromic number.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Handle out‑of‑range indices (behavior below offset is unspecified). */
    if (n == 0) return 0;

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_candidate = 1; /* next integer to test */

    /* Grow the cache as needed */
    if (n > cache_len) {
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));
        while (cache_len < n) {
            seq_t k = (seq_t)next_candidate;
            seq_t sum = 0;
            seq_t sqrtk = floor_sqrt(k);
            uint64_t sqrt_lim = (uint64_t)sqrtk;

            for (uint64_t i = 1; i <= sqrt_lim; ++i) {
                if ((k % (seq_t)i) == 0) {
                    seq_t d1 = (seq_t)i;
                    seq_t d2 = k / d1;

                    if (is_palindrome(d1, 10)) sum += d1;

                    if (d1 != d2 && is_palindrome(d2, 10)) sum += d2;
                }
            }

            if (is_palindrome(sum, 10)) {
                cache[cache_len++] = k;
            }

            ++next_candidate;
        }
    }

    return cache[n - 1];
}
