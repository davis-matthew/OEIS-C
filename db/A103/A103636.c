/*
 * A103636  a(n) = Sum_{d|n, d==0 mod 3} d^2.
 * https://oeis.org/A103636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d≡0 (mod 3)} d^2 Equivalently, if n%3==0: a(n)
 *            = 9 * sigma2(n/3), where sigma2(k) = Sum_{e|k} e^2 (sum of
 *            squares of divisors)
 * Range:     u8 n <= 17, u16 n <= 215, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 88 terms through n = 9506 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute sigma_2(k) by multiplicative prime factorisation. */
static seq_t sigma2_uint64(idx_t k)
{
    seq_t result = 1;          /* product of terms for each prime power */
    idx_t temp = k;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* term = 1 + p^2 + p^4 + ... + p^(2*exp) = sum_{i=0..exp} (p^2)^i */
            seq_t p2 = (seq_t)p * (seq_t)p;   /* p^2 as seq_t */
            seq_t term = 1;
            seq_t pow_val = 1;
            for (idx_t i = 0; i < exp; ++i) {
                pow_val = pow_val * p2;        /* (p^2)^(i+1) */
                term = term + pow_val;         /* accumulate */
            }
            result = result * term;
        }
    }

    /* temp may be a remaining prime > sqrt(k) */
    if (temp > 1) {
        seq_t p2 = (seq_t)temp * (seq_t)temp;    /* p^2 */
        seq_t term = 1 + p2;                     /* 1 + p^2 */
        result = result * term;
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n % 3 != 0)                     /* terms not divisible by 3 are zero */
        return 0;

    idx_t k = n / 3;
    seq_t sigma = sigma2_uint64(k);      /* sum of squares of divisors of k */
    seq_t nine = 9;
    return sigma * nine;                 /* 9 * sigma2(k) = a(n) */
}
