/*
 * A076346  A076341(A000961(n)), imaginary part of prime powers mapped as
 *          defined in A076340, A076341.
 * https://oeis.org/A076346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A076341(A000961(n)); for the prime power p^e =
 *            A000961(n), a(n) = Im(f(p)^e), where f(2) = (2,0), f(p) =
 *            (p-1,1) if p == 1 (mod 4), and f(p) = (p+1,-1) if p == 3 (mod
 *            4). a(1) = 0. The code finds the n-th prime power by scanning
 *            the integers in order
 * Range:     i8 n <= 32, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    idx_t p = 1, e = 0;
    for (;;) {
        bool pp = false;
        if (m == 1) {
            pp = true;
            p = 1; e = 0;
        } else {
            idx_t r = m, q = 0;
            for (idx_t d = 2; d * d <= r; d++) {
                if (r % d == 0) { q = d; break; }
            }
            if (q == 0) {
                p = m; e = 1; pp = true;
            } else {
                idx_t ee = 0;
                while (r % q == 0) { r /= q; ee++; }
                if (r == 1) { p = q; e = ee; pp = true; }
            }
        }
        if (pp) {
            count++;
            if (count == n) break;
        }
        m++;
    }
    if (m == 1 || p == 2) return 0;
    seq_t ap, bp;
    if ((p & 3) == 1) { ap = (seq_t)p - 1; bp = 1; }
    else { ap = (seq_t)p + 1; bp = -1; }
    seq_t cr = 1, ci = 0;
    for (idx_t k = 0; k < e; k++) {
        seq_t nr = cr * ap - ci * bp;
        seq_t ni = cr * bp + ci * ap;
        cr = nr; ci = ni;
    }
    return ci;
}
