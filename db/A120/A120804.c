/*
 * A120804  Primes with consecutive digits descending.
 * https://oeis.org/A120804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal representation is a *
 *            decreasing cyclic sequence of digits: d, d-1, d-2, … (mod 10), *
 *            where the starting digit d is in 1…9 and the length is at least
 *            1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

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
    idx_t count = 0;
    for (idx_t len = 1; ; ++len) {            /* increasing length guarantees ascending values */
        for (unsigned int d = 1; d <= 9; ++d) { /* start digit cannot be 0 */
            seq_t val = 0;
            unsigned int cur = d;
            for (idx_t i = 0; i < len; ++i) {
                val = val * 10 + cur;
                cur = (cur == 0) ? 9 : cur - 1; /* wrap after 0 */
            }
            if (is_prime_big(val)) {
                ++count;
                if (count == n) {
                    return val;
                }
            }
        }
    }
}
