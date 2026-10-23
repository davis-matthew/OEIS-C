/*
 * A125559  Numbers k such that 2k^3 + 7 is prime. For k >= 8, bases in which
 *          2007 is prime.
 * https://oeis.org/A125559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is the nth non‑negative integer k such that 2*k^3 + 7 is
 *            prime
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 11, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 0;

    /* generate terms until we have the nth one */
    while (len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t val = 2 * k * k * k + 7;
        if (is_prime_big(val)) {
            if (len + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory – in the unlikely event we can't succeed */
                    return 0;
                }
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = k;
        }
        next_k++;
    }
    return cache[n - 1];
}
