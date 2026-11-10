/*
 * A050667  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 5.
 * https://oeis.org/A050667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p with A048853(p) = 5; found by search over
 *            primes, counting primes obtained by altering one decimal digit
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

#define A050667_CAP 65536

static uint64_t A050667_cnt_alter(uint64_t p)
{
    uint8_t digs[20];
    int d = 0;
    uint64_t t = p;
    while (t > 0) { digs[d++] = (uint8_t)(t % 10); t /= 10; }
    /* digs[0] is least significant */
    int count = 0;
    uint64_t place = 1;
    for (int pos = 0; pos < d; ++pos) {
        int orig = digs[pos];
        uint64_t base = p - (uint64_t)orig * place;
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == orig) continue;
            if (pos == d - 1 && nd == 0) continue;
            if (is_prime_u64(base + (uint64_t)nd * place)) ++count;
        }
        place *= 10;
    }
    return (uint64_t)count;
}

seq_t a(idx_t n)
{
    static uint64_t terms[A050667_CAP];
    static idx_t found = 0;
    static uint64_t next = 2;

    if (n < 1) return 0;
    if (n <= (idx_t)A050667_CAP) {
        while (found < n) {
            uint64_t c = next++;
            if (!is_prime_u64(c)) continue;
            if (A050667_cnt_alter(c) == 5) terms[found++] = c;
        }
        return (seq_t)terms[n - 1];
    }
    /* beyond cache: scan from scratch */
    idx_t k = 0;
    uint64_t c = 2;
    for (;; ++c) {
        if (!is_prime_u64(c)) continue;
        if (A050667_cnt_alter(c) == 5 && ++k == n) return (seq_t)c;
    }
}
