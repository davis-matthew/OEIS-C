/*
 * A096524  Numbers n such that n.0.n+1 is prime, where '.' represents
 *          concatenation. Or, n such that 10^(k+1)*n + n + 1 is prime, where
 *          k is the number of digits in n.
 * https://oeis.org/A096524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A096524: numbers n such that n.0.(n+1) is prime
   (concatenation of n, 0, and n+1). Uses a brute‑force search with memoisation. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          // undefined below the offset
    }

    static seq_t *cache       = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = (seq_t)1;

    while (cache_len < (size_t)n) {
        seq_t cand   = next_candidate;
        seq_t next   = cand + (seq_t)1;      // n + 1

        int d = digit_len(next);            // digits in n+1
        seq_t mul = pow10((idx_t)(d + 1));   // 10^(digits(n+1)+1)

        seq_t p = mul * cand + next;         // concatenated number

        if (is_prime_big(p)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        next_candidate = cand + (seq_t)1;
    }

    return cache[n - 1];
}
