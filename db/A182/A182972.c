/*
 * A182972  Numerators of positive rationals < 1 arranged by increasing sum of
 *          numerator and denominator then by increasing numerator.
 * https://oeis.org/A182972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A182972: Numerator of the n‑th fraction < 1
   (Fractions enumerated by increasing sum of numerator and
   denominator, then by increasing numerator).  This implementation
   uses Euler's totient function to skip whole blocks of sums, and
   then finds the exact numerator by testing coprimeness. */

#include <stdint.h>

/* Euclidean algorithm for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Pure 64‑bit totient; the required sums are small for the tested
   ranges, so trial division is fast enough. */
static idx_t totient_u64(idx_t n) {
    idx_t result = n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

seq_t a(idx_t n) {
    /* n is 1‑based.  We walk through sums s = 3,4,5,...,
       until the n-th fraction lies in a particular block. */
    idx_t remaining = n;
    for (idx_t sum = 3; ; ++sum) {
        idx_t phi_sum = totient_u64(sum);   /* φ(sum) */
        idx_t count = phi_sum / 2;          /* Fraction count for this sum */

        if (remaining > count) {
            remaining -= count;
            continue;
        }

        /* Find the remaining‑th coprime m with 1 <= m < sum,
           such that m < sum - m (i.e., m < sum/2). */
        for (idx_t m = 1; m < sum; ++m) {
            if (gcd_seq((seq_t)m, (seq_t)sum) == 1) {
                if (m >= sum - m)    /* skip pairs with m >= denominator */
                    continue;
                if (--remaining == 0)
                    return (seq_t)m;
            }
        }
        /* The loop must terminate once we find the numerator. */
    }
}
