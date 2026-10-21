/*
 * A078747  Expansion of Sum_{k>0} k*phi(k)*x^k/(1+x^k).
 * https://oeis.org/A078747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏_{p^e||n} f(p,e) with f(2,e) = (4^e – 1) / 3, f(p,e) =
 *            (p^(2e+1) + 1) / (p + 1) for odd primes p
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A078747. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value is undefined for n<1, return 0 */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* ----- factor 2 ---------------------------------------------------- */
    idx_t e = 0;
    while ((temp & 1) == 0) {          /* while temp is even */
        temp >>= 1;                    /*   temp /= 2 */
        ++e;
    }
    if (e > 0) {
        seq_t pow4 = (seq_t)1;          /* pow4 = 4^e */
        for (idx_t i = 0; i < e; ++i) pow4 *= (seq_t)4;
        seq_t contrib = (pow4 - (seq_t)1) / (seq_t)3;
        result *= contrib;
    }

    /* ----- factor odd primes ------------------------------------------ */
    for (idx_t p = 3; p <= temp / p; p += 2) {  /* p*p ≤ temp, no overflow */
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t pow = (seq_t)1;
            for (idx_t i = 0; i < 2 * exp + 1; ++i) pow *= (seq_t)p; /* p^(2e+1) */
            seq_t numerator = pow + (seq_t)1;
            seq_t denom = (seq_t)(p + 1);
            seq_t contrib = numerator / denom;
            result *= contrib;
        }
    }

    /* ----- remaining prime factor (if any) ----------------------------- */
    if (temp > 1) {                     /* temp is a prime > sqrt(original n) */
        seq_t p = (seq_t)temp;
        seq_t pow = p * p * p;           /* p^(2*1+1) = p^3 */
        seq_t numerator = pow + (seq_t)1;
        seq_t denom = p + (seq_t)1;
        seq_t contrib = numerator / denom;
        result *= contrib;
    }

    return result;
}
