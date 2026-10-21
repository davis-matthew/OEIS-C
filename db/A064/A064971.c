/*
 * A064971  a(n) = n*usigma(n), where usigma(n) is the sum of unitary divisors
 *          of n (A034448).
 * https://oeis.org/A064971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n * Π_{p^e‖n} (p^e + 1) (multiplicative: a(p^e) =
 *            p^e*(p^e+1))
 * Range:     u8 n <= 13, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 – return 0 */

    seq_t prod = (seq_t)1;
    idx_t temp = n;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) { temp >>= 1; ++e; }
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < e; ++i) p_pow *= (seq_t)2;
        prod *= (p_pow + (seq_t)1);
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            seq_t p_pow = (seq_t)1;
            for (idx_t i = 0; i < e; ++i) p_pow *= (seq_t)p;
            prod *= (p_pow + (seq_t)1);
        }
    }

    /* remaining prime > sqrt(n) */
    if (temp > 1) {
        seq_t p_pow = (seq_t)temp;
        prod *= (p_pow + (seq_t)1);
    }

    return (seq_t)n * prod;
}
