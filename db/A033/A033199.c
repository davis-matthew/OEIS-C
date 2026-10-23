/*
 * A033199  Primes of form x^2+6*y^2.
 * https://oeis.org/A033199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 1 or 7 modulo 24 (primes of the
 *            form x^2+6*y^2)
 * Range:     u8 n <= 12, u16 n <= 1618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes */
    static seq_t *list   = NULL;
    static size_t list_cap = 0;
    static size_t list_len = 0;
    static idx_t cur = 7; /* first candidate > 5 */

    if (n == 0) return 0;    /* undefined, guard against underflow */

    /* Extend cache until it contains at least n elements */
    while (list_len < (size_t)n) {
        /* Test primality */
        if (is_prime(cur)) {
            uint64_t r = cur % 24ULL;
            if (r == 1 || r == 7) {
                /* Ensure capacity */
                if (list_len >= list_cap) {
                    size_t new_cap = list_cap ? list_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failed */
                    list = tmp;
                    list_cap = new_cap;
                }
                list[list_len++] = (seq_t)cur;
            }
        }
        cur++;
    }
    return list[n - 1];
}
