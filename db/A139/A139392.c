/*
 * A139392  Odd noncyclic numbers; odd numbers n such that gcd(n,phi(n)) > 1.
 * https://oeis.org/A139392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth odd noncyclic number: odd m such that gcd(m,phi(m))>1 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size = 0;
    static size_t cap = 0;
    static idx_t next_m = 3; /* start with the first odd >1 */

    while (size < n) {
        seq_t current = (seq_t)next_m;
        seq_t phi = calc_phi(current);
        seq_t g = gcd(current, phi);
        if (g > 1) {
            if (size == cap) {
                cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[size++] = current;
        }
        next_m += 2; /* next odd candidate */
    }
    return cache[n - 1];
}
