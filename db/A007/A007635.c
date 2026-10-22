/*
 * A007635  Primes of form n^2 + n + 17.
 * https://oeis.org/A007635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A028823(n)^2 + A028823(n) + 17; search over m >= 0 for
 *            primes m^2+m+17
 * Range:     u8 n <= 15, u16 n <= 122, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static uint64_t *ms = NULL;
    static idx_t found = 0;
    static idx_t cap = 0;
    static uint64_t next_m = 0;

    while (found < n) {
        uint64_t v = next_m * next_m + next_m + 17;
        if (is_prime(v)) {
            if (found == cap) {
                idx_t nc = cap ? cap * 2 : 256;
                ms = (uint64_t *)realloc(ms, (size_t)nc * sizeof(uint64_t));
                cap = nc;
            }
            ms[found++] = next_m;
        }
        next_m++;
    }

    seq_t m = (seq_t)ms[n - 1];
    return m * m + m + 17;
}
