/*
 * A058370  Primes p such that p and p^2 have same digit sum.
 * https://oeis.org/A058370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_sum, is_prime_big, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p such that digit_sum(p) == digit_sum(p^2) */
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t   *cache   = NULL;
    static size_t   cap     = 0;
    static size_t   len     = 0;
    /* Current candidate to test */
    static seq_t   cur     = 2;

    /* Ensure we have at least n terms cached */
    while (len < (size_t)n) {
        /* Skip even numbers greater than 2 */
        if (cur > 2 && (cur & 1) == 0) {
            cur++;
            continue;
        }

        if (is_prime_big(cur)) {
            seq_t s1 = digit_sum(cur);
            seq_t s2 = digit_sum(seq_mul(cur, cur));
            if (s1 == s2) {
                /* Grow cache if necessary */
                if (len == cap) {
                    size_t new_cap = (cap == 0) ? 8 : cap * 2;
                    seq_t *tmp = seq_realloc(cache, cap, new_cap);
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[len++] = cur;
            }
        }
        cur++;
    }

    return cache[n - 1];
}
