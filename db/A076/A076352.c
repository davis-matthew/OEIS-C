/*
 * A076352  Squarefree numbers k such that A076341(k) = 0.
 * https://oeis.org/A076352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th squarefree k with Im(f(k)) = 0, where f is
 *            multiplicative with f(2)=(2,0), f(p)=(p-1,1) for p=1 mod 4,
 *            f(p)=(p+1,-1) for p=3 mod 4 (A076341); found by incremental
 *            search with caching
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A076352_test(uint64_t k)
{
    __int128 re = 1, im = 0;
    uint64_t m = k;
    uint64_t p;
    if ((m & 1) == 0) {
        m >>= 1;
        if ((m & 1) == 0) return 0;
        re = 2;
    }
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            __int128 ap, bp, nr, ni;
            m /= p;
            if (m % p == 0) return 0;
            if ((p & 3) == 1) { ap = (__int128)p - 1; bp = 1; }
            else { ap = (__int128)p + 1; bp = -1; }
            nr = re * ap - im * bp;
            ni = re * bp + im * ap;
            re = nr; im = ni;
        }
    }
    if (m > 1) {
        __int128 ap, bp, ni;
        if ((m & 3) == 1) { ap = (__int128)m - 1; bp = 1; }
        else { ap = (__int128)m + 1; bp = -1; }
        ni = re * bp + im * ap;
        im = ni;
    }
    return im == 0;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = 0;
    static idx_t size = 0, cap = 0;
    static uint64_t last = 0;

    if (n > size) {
        if (n > cap) {
            idx_t nc = cap ? cap : 64;
            while (nc < n) nc *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
            cap = nc;
        }
        while (size < n) {
            do {
                last++;
            } while (!A076352_test(last));
            cache[size++] = last;
        }
    }
    return (seq_t)cache[n - 1];
}
