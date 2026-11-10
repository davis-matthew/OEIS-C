/*
 * A077000  a(n) = Fibonacci index of A076999(n).
 * https://oeis.org/A077000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index i of Fibonacci F(i) = A076999(n) (a(n)=1 if
 *            A076999(n)=1), where A076999(1)=1, A076999(n+1) = largest
 *            Fibonacci number <= n*A076999(n)
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A077000_L 160

static int A077000_cmp(const uint32_t *x, const uint32_t *y)
{
    for (int i = A077000_L - 1; i >= 0; --i) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint32_t f[A077000_L], g[A077000_L], t[A077000_L], s[A077000_L];
    memset(f, 0, sizeof f);
    memset(g, 0, sizeof g);
    f[0] = 1;   /* F(i)   */
    g[0] = 1;   /* F(i+1) */
    unsigned long long idx = 1;

    for (idx_t k = 2; k <= n; ++k) {
        uint64_t m = (uint64_t)(k - 1);
        /* t = f * m */
        uint64_t carry = 0;
        for (int i = 0; i < A077000_L; ++i) {
            uint64_t lo = (uint64_t)f[i] * (m & 0xffffffffULL) + carry;
            t[i] = (uint32_t)lo;
            carry = lo >> 32;
        }
        if (m > 0xffffffffULL) {
            /* not reachable for realistic n; ignore high part */
        }
        while (A077000_cmp(g, t) <= 0) {
            uint64_t c = 0;
            for (int i = 0; i < A077000_L; ++i) {
                uint64_t v = (uint64_t)f[i] + g[i] + c;
                s[i] = (uint32_t)v;
                c = v >> 32;
            }
            memcpy(f, g, sizeof f);
            memcpy(g, s, sizeof g);
            ++idx;
        }
    }
    if (idx == 2) idx = 1;
    return (seq_t)idx;
}
