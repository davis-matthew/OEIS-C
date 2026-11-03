/*
 * A156987  Primes using all the prime digits 2, 3, 5, 7 repeated in strings,
 *          in that order.
 * https://oeis.org/A156987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the n-th prime composed only of the digits 2,3,5,7
   in strictly ascending order (digits repeat but never decrease). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    /* iterate over lengths of the number (minimum 4 digits) */
    for (idx_t len = 4; ; ++len) {
        /* k2: number of leading '2's, must be at least 1 */
        for (int k2 = (int)(len - 3); k2 >= 1; --k2) {
            idx_t k2u = (idx_t)k2;
            idx_t rem2 = len - k2u;          /* remaining digits after '2' part */

            /* k3: number of '3's, at least 1 */
            for (int k3 = (int)(rem2 - 2); k3 >= 1; --k3) {
                idx_t k3u = (idx_t)k3;
                idx_t rem3 = rem2 - k3u;     /* remaining after '3's */

                /* k5: number of '5's, at least 1 */
                for (int k5 = (int)(rem3 - 1); k5 >= 1; --k5) {
                    idx_t k5u = (idx_t)k5;
                    idx_t k7u = rem3 - k5u;   /* remaining digits are '7's */

                    /* compose the decimal string */
                    char buf[512];
                    char *p = buf;
                    for (idx_t i = 0; i < k2u; ++i) *p++ = '2';
                    for (idx_t i = 0; i < k3u; ++i) *p++ = '3';
                    for (idx_t i = 0; i < k5u; ++i) *p++ = '5';
                    for (idx_t i = 0; i < k7u; ++i) *p++ = '7';
                    *p = '\0';

                    seq_t num = seq_from_str(buf);
                    if (is_prime_big(num)) {
                        ++found;
                        if (found == n) {
                            return num;
                        }
                    }
                }
            }
        }
    }
}
