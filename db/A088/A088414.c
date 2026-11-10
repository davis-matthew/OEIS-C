/*
 * A088414  A088260 indexed by A002110.
 * https://oeis.org/A088414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = k such that A088260(n) = A002110(k); search over k = 0,
 *            1, 2, ..., keeping primorial(k) as an exact decimal digit array
 *            and testing whether it is a palindrome once its trailing zeros
 *            are removed
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define A088414_MAXK 3000
#define A088414_MAXFOUND 64

static uint64_t A088414_next_prime(uint64_t last)
{
    uint64_t c = last + 1;
    for (;;) {
        bool ok = (c >= 2);
        uint64_t d;
        for (d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
        c++;
    }
}

/* digits little-endian, len digits */
static bool A088414_is_pal_trim(const uint8_t *dig, size_t len)
{
    size_t lo = 0, hi;
    while (lo < len && dig[lo] == 0) lo++;
    if (lo >= len) return true;
    hi = len - 1;
    while (lo < hi) {
        if (dig[lo] != dig[hi]) return false;
        lo++;
        hi--;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t found[A088414_MAXFOUND];
    static size_t cnt = 0;
    static bool init = false;
    static uint64_t k = 0;
    static uint64_t lastp = 1;
    static uint8_t *dig = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    if (!init) {
        cap = 1024;
        dig = (uint8_t *)malloc(cap);
        if (!dig) return 0;
        dig[0] = 1;
        len = 1;
        found[cnt++] = 0; /* primorial(0) = 1 */
        init = true;
    }

    while ((uint64_t)cnt < n && cnt < A088414_MAXFOUND && k < A088414_MAXK) {
        uint64_t p = A088414_next_prime(lastp);
        uint64_t carry = 0;
        size_t i;
        lastp = p;
        k++;
        for (i = 0; i < len; i++) {
            uint64_t v = (uint64_t)dig[i] * p + carry;
            dig[i] = (uint8_t)(v % 10);
            carry = v / 10;
        }
        while (carry > 0) {
            if (len == cap) {
                size_t nc = cap * 2;
                uint8_t *nd = (uint8_t *)realloc(dig, nc);
                if (!nd) return 0;
                dig = nd;
                cap = nc;
            }
            dig[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
        if (A088414_is_pal_trim(dig, len))
            found[cnt++] = k;
    }

    if (n >= 1 && n - 1 < (idx_t)cnt) return (seq_t)found[n - 1];
    return 0;
}
