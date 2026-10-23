/*
 * A020479  Number of noninvertible 2 X 2 matrices over Z/nZ (determinant is a
 *          divisor of 0).
 * https://oeis.org/A020479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - ∏_{p^e‖n} p^{4(e-1)} (p^4 - p^3 - p^2 + p)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (number of noninvertible 2×2 matrices over ℤ/nℤ)           */

static seq_t pow_int_seq(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2u) return 0;                /* offset is 2; behaviour below 2 is irrelevant */

    /* compute n^4 */
    seq_t n4 = (seq_t)n;
    n4 *= (seq_t)n;
    n4 *= (seq_t)n;
    n4 *= (seq_t)n;

    /* product of |GL(2,ℤ/p^eℤ)| for each prime power dividing n */
    seq_t prod = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) continue;
        int exp = 0;
        while (temp % p == 0) { temp /= p; ++exp; }

        seq_t sp = (seq_t)p;
        seq_t pow_4e4 = pow_int_seq(sp, 4 * (exp - 1));   /* p^{4(e-1)}  */
        seq_t inner   = pow_int_seq(sp, 4) -                /* p^4      */
                        pow_int_seq(sp, 3) -                /* p^3      */
                        pow_int_seq(sp, 2) + sp;             /* + p      */
        prod *= pow_4e4 * inner;
    }

    /* remaining prime factor (if any), exponent 1 */
    if (temp > 1) {
        seq_t sp = (seq_t)temp;
        seq_t inner = pow_int_seq(sp, 4) - pow_int_seq(sp, 3) - pow_int_seq(sp, 2) + sp;
        prod *= inner;
    }

    return n4 - prod;
}
