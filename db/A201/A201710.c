/*
 * A201710  Primes of the form 10n^2 + 3.
 * https://oeis.org/A201710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 10 n^2 + 3 (OEIS A201710)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 2269, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 0;

    while (len < n) {
        seq_t k = (seq_t)next_k;
        seq_t candidate = ((seq_t)10 * k * k) + (seq_t)3;

        if (isPrimeGeneric(candidate)) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;              /* realloc failed: return 0 */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
