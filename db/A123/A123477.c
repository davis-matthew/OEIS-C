/*
 * A123477  Expansion of (1 - b(q)) / 3 in powers of q where b(q) is a cubic
 *          AGM theta function.
 * https://oeis.org/A123477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e in n of * a(p^e) = { -2 if
 *            p = 3 * e+1 if p ≡ 1 (mod 6) * (1 + (-1)^e)/2 = 1 if p ≡ 2 (mod
 *            6) and e even, * 0 if p ≡ 2 (mod 6) and e odd * } and a(1) = 1.
 *            * The sequence is multiplicative with these local factors
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {            /* not part of the sequence, but safe */
        return 0;
    }
    seq_t result = 1;
    idx_t m = n;

    /* factor by trial division, efficient enough for n ≤ about 10⁶ per call */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t term;
            if (p == 3) {
                term = (seq_t)-2;
            } else if (p % 6 == 1) {
                term = (seq_t)exp + 1;
            } else {   /* p ≡ 2 or 5 (mod 6), but primes ≥ 2 => only 2 or 5 mod 6 */
                term = (exp % 2 == 0) ? 1 : 0;
            }
            result *= term;
            if (result == 0) {
                break;   /* no need to process further, product will remain zero */
            }
        }
    }

    /* if remainder is a prime > sqrt(original n) */
    if (m > 1) {
        seq_t term;
        if (m == 3) {
            term = (seq_t)-2;
        } else if (m % 6 == 1) {
            /* exponent is 1 */
            term = 1 + 1; /* 1 + 1 = 2 */
        } else {
            /* exponent is odd (1), so (1 + (-1)^1)/2 = 0 */
            term = 0;
        }
        result *= term;
    }
    return result;
}
