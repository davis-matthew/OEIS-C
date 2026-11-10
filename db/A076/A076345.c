/*
 * A076345  A076340(A000961(n)), real part of prime powers mapped as defined
 *          in A076340, A076341.
 * https://oeis.org/A076345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = real part of f(q), where q = A000961(n) = p^k (q=1 gives
 *            1), f(p^k) = f(p)^k in Z[i], f(2) = (2,0), f(p) = (p-1,1) if p
 *            == 1 (mod 4), f(p) = (p+1,-1) if p == 3 (mod 4). The n-th prime
 *            power is found with a sieve whose limit doubles until it holds n
 *            terms
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static idx_t A076345_nth_prime_power(idx_t n)
{
    idx_t limit = 128;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
        unsigned char *pp = (unsigned char *)malloc((size_t)limit + 1);
        memset(comp, 0, (size_t)limit + 1);
        memset(pp, 0, (size_t)limit + 1);
        pp[1] = 1;
        for (idx_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i) comp[j] = 1;
                }
                idx_t q = i;
                for (;;) {
                    pp[q] = 1;
                    if (q > limit / i) break;
                    q *= i;
                }
            }
        }
        idx_t count = 0, found = 0;
        for (idx_t i = 1; i <= limit; i++) {
            if (pp[i]) {
                count++;
                if (count == n) { found = i; break; }
            }
        }
        free(comp);
        free(pp);
        if (found) return found;
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    idx_t v = A076345_nth_prime_power(n);
    if (v == 1) return (seq_t)1;

    idx_t p = v;
    for (idx_t d = 2; d <= v / d; d++) {
        if (v % d == 0) { p = d; break; }
    }
    idx_t k = 0;
    while (v % p == 0) { v /= p; k++; }

    seq_t fr, fi;
    if (p == 2) {
        fr = (seq_t)2; fi = (seq_t)0;
    } else if (p % 4 == 1) {
        fr = (seq_t)p - 1; fi = (seq_t)1;
    } else {
        fr = (seq_t)p + 1; fi = (seq_t)0 - (seq_t)1;
    }

    seq_t rr = 1, ri = 0;
    for (idx_t t = 0; t < k; t++) {
        seq_t nr = rr * fr - ri * fi;
        seq_t ni = rr * fi + ri * fr;
        rr = nr;
        ri = ni;
    }
    return rr;
}
