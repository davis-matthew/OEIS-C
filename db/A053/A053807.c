/*
 * A053807  a(n) = Sum_{k=1..n, n mod k = 1} k^2.
 * https://oeis.org/A053807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sigma_2(n-1) - 1 for n >= 2, a(1) = 0 where sigma_2(m) =
 *            sum of squares of all divisors of m
 * Range:     u8 n <= 15, u16 n <= 216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {            /* a(1) = 0, a(0) not used */
        return 0;
    }

    idx_t m      = n - 1;    /* we need sigma_2(m) */
    seq_t total  = 1;        /* multiplicative identity */
    idx_t temp   = m;        /* copy for factorisation */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Compute term = 1 + p^2 + p^4 + ... + p^{2e} */
            seq_t p2   = (seq_t)p * (seq_t)p;
            seq_t term = 1;
            seq_t curr = 1;
            for (unsigned j = 0; j < e; ++j) {
                curr *= p2;    /* curr = p^{2(j+1)} */
                term += curr;  /* add to sum */
            }
            total *= term;         /* multiply by this prime's contribution */
        }
    }

    /* If a prime factor > sqrt(m) remains */
    if (temp > 1) {
        seq_t p2   = (seq_t)temp * (seq_t)temp;
        seq_t term = 1 + p2;
        total *= term;
    }

    /* subtract the 1 corresponding to divisor 1 */
    return total - 1;
}
