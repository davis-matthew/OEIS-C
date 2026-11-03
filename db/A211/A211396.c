/*
 * A211396  Smallest prime substring of n, or 0 if no such substring exists.
 * https://oeis.org/A211396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 400, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9624 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A211396: Smallest prime substring of n, or 0 if none.
   Implementation uses brute‑force generation of all decimal
   substrings and tests each for primality. */

#include <stdbool.h>
#include <stddef.h>
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

static void idx_to_string(idx_t n, char *buf, size_t *len) {
    if (n == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[32];
    int cnt = 0;
    while (n > 0) {
        tmp[cnt++] = '0' + (int)(n % 10);
        n /= 10;
    }
    for (int i = 0; i < cnt; ++i)
        buf[i] = tmp[cnt - 1 - i];
    *len = cnt;
}

seq_t a(idx_t n) {
    char s[32];
    size_t len;
    idx_to_string(n, s, &len);

    seq_t best = 0;
    bool found = false;

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + (seq_t)(s[j] - '0');
            if (is_prime_big(val)) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }
    }
    return found ? best : (seq_t)0;
}
