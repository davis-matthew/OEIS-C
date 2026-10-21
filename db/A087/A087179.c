/*
 * A087179  a(n) = (...(((x1^x2)^x3)^x4)^...) where x1,x2,... are the
 *          exponents in the prime factorization of n, a(1) = 0.
 * https://oeis.org/A087179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the power tower of the exponents of n in its prime *
 *            factorisation: a(n) = ((x1^x2)^x3)^x4^... (a(1)=0) * where n = Π
 *            p_i^x_i with p_1 < p_2 < …
 * Range:     u8 n <= 1295, u16 n <= 9474, u32 n <= 9474, u64 n <= 9474,
 *            u128 n <= 9474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n == 1 is the special initial value */
    if (n == 1) return 0;

    /* Prime factorisation – exponents are stored in ascending prime order.
       For n ≤ 10000 the number of distinct primes is below 15, so a static
       array of size 20 is sufficient. */
    int exps[20];
    int exps_cnt = 0;

    idx_t m = n;
    /* factor 2 */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) exps[exps_cnt++] = e;

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[exps_cnt++] = e;
        }
    }
    /* any remaining factor is a prime > sqrt(n) */
    if (m > 1) {
        exps[exps_cnt++] = 1;
    }

    /* Evaluate the power tower from left to right:
         ((x1^x2)^x3)^x4^... */
    seq_t result = exps[0];
    for (int i = 1; i < exps_cnt; ++i) {
        seq_t base = result;
        int exp = exps[i];
        seq_t r = 1;
        for (int j = 0; j < exp; ++j) {
            r *= base;
        }
        result = r;
    }

    return result;
}
