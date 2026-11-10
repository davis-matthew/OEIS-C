/*
 * A078400  Iterated sum-of-digits of A078403(n).
 * https://oeis.org/A078400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital root of A078403(n), where A078403 lists the
 *            primes whose digital root is prime; computed by a sieve
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t limit = 64;
    uint64_t need = (uint64_t)n;
    while (true) {
        unsigned char *comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!comp) return (seq_t)0;
        uint64_t cnt = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                if (i * i <= limit)
                    for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
                uint64_t dr = (i - 1) % 9 + 1;
                if (dr == 2 || dr == 3 || dr == 5 || dr == 7) {
                    cnt++;
                    if (cnt == need) {
                        free(comp);
                        return (seq_t)dr;
                    }
                }
            }
        }
        free(comp);
        limit *= 2;
    }
}
