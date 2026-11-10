/*
 * A214788  Numbers n such that n-3=A214787(n).
 * https://oeis.org/A214788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A214787(m) = m-3, found by search
 *            (fast-doubling Fibonacci test, then minimality check)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

typedef unsigned __int128 A214788_u128;

static void A214788_fibpair(uint64_t k, uint64_t m, uint64_t *f, uint64_t *g)
{
    /* (F(k), F(k+1)) mod m */
    uint64_t a = 0 % m, b = 1 % m;
    int i;
    for (i = 63; i >= 0; --i) {
        A214788_u128 t = (A214788_u128)b * 2;
        uint64_t t2 = (uint64_t)(t % m);
        uint64_t d = (t2 + m - a) % m;
        uint64_t c = (uint64_t)(((A214788_u128)a * d) % m);
        uint64_t e = (uint64_t)((((A214788_u128)a * a) + ((A214788_u128)b * b)) % m);
        if ((k >> i) & 1) { a = e; b = (c + e) % m; }
        else { a = c; b = e; }
    }
    *f = a; *g = b;
}

static bool A214788_test(uint64_t m)
{
    uint64_t f, g;
    if (m < 5) return false;
    A214788_fibpair(m - 4, m, &f, &g);
    if ((f + 5) % m != 0) return false;
    {
        uint64_t p = 0, c = 1 % m, j;
        for (j = 1; j + 5 <= m; ++j) {
            if ((c + 5) % m == 0) return false;
            uint64_t nx = (p + c) % m;
            p = c; c = nx;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t terms[4096];
    static idx_t count = 0;
    static uint64_t next = 5;
    if (n == 0) return 0;
    while (count < n && count < 4096) {
        if (A214788_test(next)) terms[count++] = next;
        ++next;
    }
    if (n > 4096) return 0;
    return (seq_t)terms[n - 1];
}
