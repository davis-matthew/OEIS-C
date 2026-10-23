/*
 * A143521  Expansion of g.f. Sum_{k>0} k * x^k / (1 + (-x)^k)^2.
 * https://oeis.org/A143521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with * a(2^e) = (3‑e) * 2^e (e > 0) *
 *            a(p^e) = (e+1) * p^e for odd primes p * a(16*n+8) = 0 (follows
 *            from the 2‑adic rule)
 * Range:     i8 n <= 29, i16 n <= 1385, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, offset starts at 1 */

    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* compute p^e */
            seq_t powp = 1;
            for (idx_t i = 0; i < e; ++i) powp *= (seq_t)p;

            seq_t term;
            if (p == 2) {
                term = ((seq_t)3 - (seq_t)e) * powp;  /* (3-e)*2^e */
            } else {
                term = ((seq_t)e + (seq_t)1) * powp;   /* (e+1)*p^e */
            }
            result *= term;
        }
    }

    if (tmp > 1) {                 /* remaining prime factor (e = 1) */
        seq_t powp = (seq_t)tmp;   /* tmp^1 */
        seq_t term = ((seq_t)1 + (seq_t)1) * powp;   /* (1+1)*p */
        result *= term;
    }

    return result;
}
