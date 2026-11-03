/*
 * A000926  Euler's "numerus idoneus" (or "numeri idonei", or idoneal, or
 *          suitable, or convenient numbers).
 * https://oeis.org/A000926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, nice
 * Formula:   search: n is a term iff every primitive reduced form
 *            a*x^2+b*x*y+c*y^2 of discriminant -4n has b=0, a=b or a=c (Cox);
 *            i.e. there is no k>=1, a with 2k<a, a^2<n+k^2, a | n+k^2,
 *            gcd(a,2k,(n+k^2)/a)=1. The sequence is finite (65 terms); 0 is
 *            returned beyond it
 * Range:     u8 n <= 50, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

static bool a926_idoneal(uint64_t n)
{
    uint64_t k;
    for (k = 1; 3 * k * k <= n; k++) {
        uint64_t m = n + k * k;
        uint64_t a;
        for (a = 2 * k + 1; a * a < m; a++) {
            if (m % a) continue;
            {
                uint64_t c = m / a;
                if (gcd_u64(gcd_u64(a, 2 * k), c) == 1) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t terms[128];
    static int count = 0;
    static uint64_t next = 1;
    static bool done = false;
    if (n == 0) return (seq_t)0;
    while ((idx_t)count < n && !done) {
        if (next > 20000) { done = true; break; }
        if (a926_idoneal(next) && count < 128) terms[count++] = next;
        next++;
    }
    if ((idx_t)count < n) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
