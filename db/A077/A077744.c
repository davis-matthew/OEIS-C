/*
 * A077744  Smallest number whose cube ends in n, or 0 if no such number
 *          exists. a(n) = A077743(n)^(1/3).
 * https://oeis.org/A077744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest integer x>0 such that x^3 ends with the decimal
 *            representation of n, or 0 if no such x exists. The value is
 *            computed by brute‑forcing all x modulo 10^d where d = number of
 *            decimal digits of n. For each x it tests (x^3 mod 10^d) == n.
 *            The result is the first (smallest) x that meets...
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 1106, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */

static int digits_of_seq_t(seq_t x) {
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x > 0);
    return d;
}

seq_t a(idx_t n) {
    /* Convert the index to a seq_t */
    seq_t sn = (seq_t)n;
    /* Number of decimal digits of n (offset starts at 1) */
    int d = digits_of_seq_t(sn);
    /* mod = 10^d as seq_t */
    seq_t mod = pow10(d);

    /* For safety we work only when mod fits into idx_t (i.e., d <= 18) */
    /* 10^18 < 2^63, so we can cast to uint64_t for the loop bound. */
    if (d > 18) return 0;   /* too large; no brute‑force feasible */

    uint64_t bound = (uint64_t)mod;   /* bound = 10^d */

    for (idx_t x = 1; x < bound; x++) {
        seq_t xx = (seq_t)x;
        seq_t cube = xx * xx;          /* x^2 */
        cube = cube * xx;              /* x^3 */
        if ((cube % mod) == sn) {
            return xx;                 /* first (smallest) solution */
        }
    }
    return 0;                          /* no solution */
}
