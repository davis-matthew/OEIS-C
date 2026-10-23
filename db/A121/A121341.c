/*
 * A121341  Number of decimal places before 1/n either recurs or terminates.
 * https://oeis.org/A121341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = A051628(n) + A051626(n)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of decimal places before 1/n either recurs or terminates. */
#include <stdint.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n) {
    /* Count powers of 2 and 5 in n. */
    idx_t e2 = 0, e5 = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        e2++;
    }
    while (t % 5ULL == 0) {
        t /= 5ULL;
        e5++;
    }

    /* Non‑repeating part length. */
    idx_t nonrep = e2 > e5 ? e2 : e5;

    /* Remaining part m is coprime to 10. */
    if (t == 1ULL) {
        return (seq_t)nonrep;
    }

    seq_t M = (seq_t)t;          /* modulus as seq_t for big‑int support   */
    seq_t r = 10 % M;            /* first remainder                       */
    idx_t period = 1;            /* period length of 1/m                  */

    while (r != 1) {
        r = (r * 10) % M;
        period++;
        /* In theory period <= phi(m) <= m-1, so order is bounded. */
    }

    return (seq_t)(nonrep + period);
}
