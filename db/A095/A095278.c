/*
 * A095278  Numbers k such that 4k + 3 is prime.
 * https://oeis.org/A095278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p - 3)/4 where p is the n-th prime congruent to 3 (mod
 *            4)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return (n == 2 || n == 3);
    for (uint64_t d = 5; d <= n / d; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;     /* 4·0+3 = 3 is prime */

    idx_t count = 1;                 /* already found k=0 */
    uint64_t k = 1;                  /* candidate k */

    for (;;) {
        uint64_t p = 4 * k + 3;      /* candidate prime */
        if (isPrime64(p)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
