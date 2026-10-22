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
 * Helpers:   a926_gcd, a926_idoneal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a926_gcd.h"
#include "../utils/a926_idoneal.h"

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
