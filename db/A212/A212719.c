/*
 * A212719  Numbers n with property that the subset A039703(1..n) contains
 *          equal numbers of odd and even terms.
 * https://oeis.org/A212719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index m such that among A039703(1..m) (primes mod 5)
 *            the numbers of odd and even terms are equal; computed by sieving
 *            primes and scanning a running odd/even balance
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t limit = 4096;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
        if (!comp) return 0;
        for (size_t i = 2; i * i <= limit; i++) {
            if (!comp[i]) {
                for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        long long bal = 0;
        idx_t idx = 0, hits = 0;
        for (size_t p = 2; p <= limit; p++) {
            if (comp[p]) continue;
            idx++;
            if ((p % 5) & 1) bal++; else bal--;
            if (bal == 0) {
                hits++;
                if (hits == n) {
                    free(comp);
                    return (seq_t)idx;
                }
            }
        }
        free(comp);
        limit *= 4;
    }
}
