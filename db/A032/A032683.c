/*
 * A032683  Numbers k such that k surrounded by digit '3' is a prime.
 * https://oeis.org/A032683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that 3k3 (decimal) is prime
 * Range:     u8 none, u16 n <= 256, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_big, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static idx_t found = 0;
    static idx_t last_k = 0;
    static size_t cap = 0;
    static seq_t *cache = NULL;

    if (cache == NULL) {
        cap = 32;
        cache = seq_alloc(cap);
        found = 0;
        last_k = 0;
    }

    if (n <= found) {
        return cache[n - 1];
    }

    for (idx_t k = last_k + 1;; ++k) {
        int d = digits(k);
        seq_t num = ((seq_t)3) * pow10((idx_t)(d + 1)) + ((seq_t)k) * (seq_t)10 + (seq_t)3;

        if (is_prime_big(num)) {
            if (found == cap) {
                size_t new_cap = cap * 2;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[found] = (seq_t)k;
            ++found;
            last_k = k;

            if (found >= n) {
                return (seq_t)k;
            }
        }
    }
}
