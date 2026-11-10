/*
 * A050653  Numbers n such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 2.
 * https://oeis.org/A050653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 with A048853(m) = 2; found by a linear
 *            search, where A048853(m) counts primes != m obtained by changing
 *            one digit of m
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

static bool A050653_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A050653_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A050653_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;

        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            ex >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t A048853_a(seq_t m)
{
    int d = num_digits(m);
    uint8_t digs[40];
    seq_t tmp = m;
    for (int i = d - 1; i >= 0; --i) {
        digs[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t count = 0;
    for (int pos = 0; pos < d; ++pos) {
        uint8_t orig = digs[pos];
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == orig) continue;
            if (pos == 0 && nd == 0) continue;
            seq_t cand = 0;
            for (int i = 0; i < d; ++i) {
                uint8_t dig = (i == pos) ? (uint8_t)nd : digs[i];
                cand = cand * 10 + dig;
            }
            if (A050653_is_prime_big(cand))
                ++count;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t m = 0;
    while (found < n) {
        ++m;
        if (A048853_a(m) == 2) ++found;
    }
    return m;
}
