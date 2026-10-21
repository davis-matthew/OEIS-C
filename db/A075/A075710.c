/*
 * A075710  Numerator of Sum_{k=1..n} frac(n/k), where frac(x/y) denotes the
 *          fractional part of x/y.
 * https://oeis.org/A075710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∑_{k=1..n} frac(n/k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively add each fractional part in lowest terms */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined, but return 0 for safety */
    if (n == 0) {
        return 0;
    }

    seq_t num = 0;   /* numerator of the running sum */
    seq_t den = 1;   /* denominator of the running sum (always reduced) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;                /* remainder n%k */
        seq_t kseq = (seq_t)k;
        seq_t rseq = (seq_t)r;

        /* Reduce the addition before multiplying to keep intermediates smaller */
        seq_t g1 = gcd(den, kseq);          /* gcd(den, k) */
        seq_t den_div = den / g1;            /* den/g1 */
        seq_t k_div  = kseq / g1;            /* k/g1 */

        seq_t new_num = num * k_div + rseq * den_div;
        seq_t new_den = den_div * kseq;      /* lcm(den,k) = den/g1 * k */

        seq_t g2 = gcd(new_num, new_den);   /* reduce the fraction */
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;  /* numerator of the reduced fraction */
}
