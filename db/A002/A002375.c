/*
 * A002375  From Goldbach conjecture: number of decompositions of 2n into an
 *          unordered sum of two odd primes.
 * https://oeis.org/A002375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   a(n) = #{(p,q): p <= q odd primes, p + q = 2n}; computed by
 *            sieving primes up to 2n and counting pairs
 * Range:     u8 n <= 3464, u16 n <= 9280, u32 n <= 9280, u64 n <= 9280,
 *            u128 n <= 9280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t target, i, j, p;
    bool *comp;

    if (n < 3) return count;
    target = (uint64_t)n * 2;

    comp = (bool *)calloc((size_t)(target + 1), sizeof(bool));
    if (!comp) return count;
    comp[0] = comp[1] = true;
    for (i = 2; i * i <= target; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= target; j += i) comp[j] = true;
        }
    }

    for (p = 3; p <= target / 2; p += 2) {
        if (!comp[p] && !comp[target - p]) count++;
    }

    free(comp);
    return count;
}
