/*
 * A203640  Length of the cycle reached for the map x->A203639(x), starting at
 *          n.
 * https://oeis.org/A203640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = length of the cycle reached by iterating x -> A203639(x)
 *            from n, where A203639 is multiplicative with A203639(p^e) =
 *            e*p^(e-1); found by Brent cycle detection
 * Range:     u8 n <= 1619, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 106 terms through n = 9833 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A203639_f(seq_t x) {
    seq_t result = 1;
    seq_t rem = x;
    if (rem < 1) return 0;
    for (seq_t p = 2; p <= rem / p; p += (p == 2 ? 1 : 2)) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            seq_t term = (seq_t)e;
            for (unsigned i = 1; i < e; i++) term *= p;
            result *= term;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t x0 = (seq_t)n;
    unsigned long long power = 1, lam = 1;
    seq_t tortoise = x0;
    seq_t hare = A203639_f(x0);
    while (tortoise != hare) {
        if (power == lam) {
            tortoise = hare;
            power *= 2;
            lam = 0;
        }
        hare = A203639_f(hare);
        lam++;
    }
    return (seq_t)lam;
}
