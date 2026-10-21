/*
 * A083266  Sum of related numbers (counted in A073757) belonging to n: a(n) =
 *          A000203(n) + A023896(n) - 1; related = {divisor-set, RRS}.
 * https://oeis.org/A083266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = sigma(n) + phi_sum(n) - 1 * where sigma(n) is the sum of
 *            divisors of n and * phi_sum(n) is the sum of integers 1 ≤ m ≤ n
 *            with gcd(m,n)=1. * This equals the sum of all m ≤ n such that
 *            gc—dm(m,n)=1 or m|n, * counted once
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* convert index to seq_t for arithmetic   */
    idx_t   temp = n;              /* working copy for factorisation          */
    seq_t   sigma = 1;             /* product of divisor sums                 */
    seq_t   phi   = n_t;           /* φ(n) – Euler's totient function         */

    /* trial division to factor n */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t pow  = 1;        /* current power of prime p              */
            seq_t seg  = 1;        /* sum of powers p^0 + … + p^e           */

            while (temp % p == 0) {
                temp /= p;
                pow  *= (seq_t)p;
                seg  += pow;
            }
            sigma *= seg;           /* multiply σ component                    */
            phi    = phi / (seq_t)p * ((seq_t)p - 1); /* φ component */
        }
    }

    /* if a prime factor remains */
    if (temp > 1) {
        seq_t p_val = (seq_t)temp;
        seq_t seg   = 1 + p_val;   /* (p^0 + p^1) */
        sigma *= seg;
        phi    = phi / p_val * (p_val - 1);
    }

    /* sum of totatives: n*φ(n)/2, except for n=1 */
    seq_t phi_sum;
    if (n_t == 1) {
        phi_sum = 1;
    } else {
        phi_sum = (n_t * phi) / 2;
    }

    return sigma + phi_sum - 1;      /* a(n) = σ(n) + φ_sum(n) - 1              */
}
