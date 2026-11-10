/*
 * A220118  First occurrence of -n in A220115.
 * https://oeis.org/A220118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k such that A007895(k) - A000120(k) = n (A220115(k)
 *            = -n); found by an incremental scan using Zeckendorf counts z(k)
 *            = z(k - F) + 1, where F is the largest Fibonacci number <= k
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static unsigned char *z = NULL;
    static uint64_t cap = 0, cnt = 0;
    static uint64_t first[128];
    static bool found[128];
    static uint64_t fib[100];
    static int nf = 0, fi = 0;
    static bool init = false;

    if (n >= 128) return (seq_t)0;

    if (!init) {
        init = true;
        fib[0] = 1; fib[1] = 2; nf = 2;
        while (nf < 90) { fib[nf] = fib[nf-1] + fib[nf-2]; nf++; }
        cap = 1 << 16;
        z = (unsigned char *)malloc((size_t)cap);
        z[0] = 0;
        cnt = 1;
        found[0] = true;
        first[0] = 0;
        fi = 0;
    }

    while (!found[n]) {
        uint64_t k = cnt;
        if (k >= cap) {
            cap *= 2;
            z = (unsigned char *)realloc(z, (size_t)cap);
        }
        while (fi + 1 < nf && fib[fi + 1] <= k) fi++;
        z[k] = (unsigned char)(z[k - fib[fi]] + 1);
        cnt++;
        int d = (int)z[k] - __builtin_popcountll((unsigned long long)k);
        if (d >= 0 && d < 128 && !found[d]) {
            found[d] = true;
            first[d] = k;
        }
    }
    return (seq_t)first[n];
}
