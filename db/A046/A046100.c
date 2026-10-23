/*
 * A046100  Biquadratefree numbers: numbers that are not divisible by any 4th
 *          power greater than 1.
 * https://oeis.org/A046100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest x such that the count of numbers * ≤ x not
 *            divisible by any 4th power ≥ n. * Here a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=4, a(5)=5, a(6)=6, a(7)=7, * a(8)=8, a(9)=9, a(10)=10,
 *            a(11)=11, a(12)=12, a(13)=13, * a(14)=14, a(15)=15, a(16)=17, …
 *            (OEIS A046100)
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------- helpers ---------- */

/* fourth power, assumes seq_t is an unsigned integer type   */
static inline seq_t pow4(seq_t a) {
    return (a * a) * (a * a);
}

/* smallest d such that d^4 > x, iterated from 0 upwards   */
static idx_t floor_fourth_root(seq_t x) {
    seq_t d = 0;
    while (1) {
        seq_t d_next = d + 1;
        seq_t d4 = pow4(d_next);
        if (d4 > x)
            break;
        d = d_next;
    }
    return (idx_t)d;          /* d fits in idx_t (very small for our n) */
}

/* Möbius function µ(d) for d≥1, computed by trial division      */
static int8_t mobius(idx_t d) {
    if (d == 1) return 1;
    int8_t sign = 1;
    for (idx_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            d /= p;
            if (d % p == 0)          /* square factor → µ=0 */
                return 0;
            sign = -sign;
            while (d % p == 0)       /* remove all copies of this prime */
                d /= p;
        }
    }
    if (d > 1)                      /* remaining prime factor */
        sign = -sign;
    return sign;
}

/* count of biquadratefree numbers ≤ x                             */
static seq_t count_biqfree(seq_t x) {
    if (x == (seq_t)0) return (seq_t)0;
    idx_t dmax = floor_fourth_root(x);
    seq_t total = 0;
    for (idx_t d = 1; d <= dmax; ++d) {
        int8_t md = mobius(d);
        if (md == 0) continue;
        seq_t term = x / pow4(d);
        if (md == 1)
            total += term;
        else
            total -= term;
    }
    return total;
}

/* ---------- required function ---------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* out of bounds, not used */
    seq_t target = (seq_t)n;
    seq_t low = (seq_t)1;
    seq_t high = target + target;         /* start with x≈2n, more than enough */

    /* Expand high until it contains at least 'n' biquadratefree numbers */
    while (count_biqfree(high) < target)
        high += high;                     /* double high */

    /* Binary search for the minimal x with count(x) ≥ n */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        if (count_biqfree(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
