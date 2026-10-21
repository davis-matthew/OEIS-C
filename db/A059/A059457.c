/*
 * A059457  Numerator of Sum_{k=0..n} (-1)^k/(3*k+1).
 * https://oeis.org/A059457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{k=0..n} (-1)^k/(3*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euclidean GCD for arbitrary unsigned integers */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t num = (seq_t)0;  /* numerator of the current partial sum   */
    seq_t den = (seq_t)1;  /* denominator of the current partial sum */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        /* denominator of the new term (-1)^k/(3*k+1) */
        seq_t term_den = (seq_t)3 * (seq_t)k + (seq_t)1;

        /* compute the new common denominator efficiently */
        seq_t g1 = seq_gcd(den, term_den);
        seq_t t1 = term_den / g1;     /* multiplier for current numerator  */
        seq_t t2 = den / g1;          /* multiplier for the new term       */
        seq_t new_den = den / g1 * term_den; /* lcm(den, term_den) */

        /* add the new term: sign = +1 for even k, -1 for odd k */
        seq_t new_num;
        if ((k & (idx_t)1) == 0) {          /* even k :  + 1/term_den   */
            new_num = num * t1 + t2;
        } else {                            /* odd  k :  - 1/term_den   */
            new_num = num * t1 - t2;
        }

        /* reduce the fraction before the next iteration */
        seq_t g2 = seq_gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* numerator of the reduced fraction */
}
