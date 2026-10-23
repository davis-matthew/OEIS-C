/*
 * A171405  Sum of divisors of n, excluding divisors 2 and 3.
 * https://oeis.org/A171405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - (n%2==0?2:0) - (n%3==0?3:0) where sigma(n) is
 *            * the sum of all positive divisors of n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, return 0 for safety */
        return 0;

    idx_t n_tmp = n;
    seq_t sigma = 1;

    for (idx_t p = 2; (idx_t)p * (idx_t)p <= n_tmp; ++p) {
        if (n_tmp % p == 0) {
            idx_t exp = 0;
            while (n_tmp % p == 0) {
                n_tmp /= p;
                ++exp;
            }
            /* compute 1 + p + p^2 + ... + p^exp */
            seq_t sum = 1;
            seq_t mult = 1;
            for (idx_t i = 1; i <= exp; ++i) {
                mult *= (seq_t)p;
                sum += mult;
            }
            sigma *= sum;
        }
    }

    if (n_tmp > 1)          /* remaining prime factor */
        sigma *= ((seq_t)n_tmp + 1);

    /* exclude 2 and 3 if they divide n */
    if (n % 2 == 0)
        sigma -= (seq_t)2;
    if (n % 3 == 0)
        sigma -= (seq_t)3;

    return sigma;
}
