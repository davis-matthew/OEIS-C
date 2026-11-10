/*
 * A081213  Let r(n,k) = if k=0 then n, else r(A081210(n),k-1), then a(n)=r(n,
 *          A081212(n)).
 * https://oeis.org/A081213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fixed point reached by iterating A081210 from n (A081210
 *            multiplicative; a(p^e) = greatest squarefree <= p^e), i.e.
 *            r(n,k) with k = A081212(n) steps
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9726 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A081213_sqfree(uint64_t n)
{
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % (d * d) == 0) return false;
    }
    return true;
}

static uint64_t A081213_step(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t res = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0, pe = 1;
            while (m % p == 0) {
                m /= p;
                e++;
                pe *= p;
            }
            if (e == 1) {
                res *= p;
            } else {
                uint64_t k = pe;
                while (!A081213_sqfree(k)) --k;
                res *= k;
            }
        }
    }
    if (m > 1) res *= m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    for (;;) {
        uint64_t y = A081213_step(x);
        if (y == x) break;
        x = y;
    }
    return (seq_t)x;
}
