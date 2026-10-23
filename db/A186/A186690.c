/*
 * A186690  Expansion of - (1/8) theta_3''(0, q) / theta_3(0, q) in powers of
 *          q.
 * https://oeis.org/A186690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, mult
 * Formula:   a(n) = Π_{p^e || n} a(p^e), where a(2^e) = -(2^e) (e>0), and
 *            a(p^e) = (p^(e+1)-1)/(p-1) for odd primes p
 * Range:     i8 n <= 89, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 1. */
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;               /* work in 64‑bit for factoring */

    /* Handle factor 2 separately. */
    if ((m & 1) == 0) {
        int e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t pow2 = 1;
        for (int i = 0; i < e; ++i) pow2 *= 2;   /* compute 2^e */
        result *= -pow2;                       /* a(2^e) = -(2^e) */
    }

    /* Factor odd primes up to sqrt(m). */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* a(p^e) = 1 + p + p^2 + ... + p^e  (odd prime) */
            seq_t term = 1;
            seq_t sum  = 1;
            for (int i = 0; i < e; ++i) {
                term *= (seq_t)p;      /* p^i */
                sum  += term;          /* add p^i */
            }
            result *= sum;
        }
    }

    /* Remaining m > 1 is an odd prime with exponent 1. */
    if (m > 1) {
        seq_t sum = 1;
        sum += (seq_t)m;                 /* 1 + p */
        result *= sum;
    }

    return result;
}
