/*
 * A066275  Number of endofunctions of [n] such that some element is fixed,
 *          but 1 is not fixed.
 * https://oeis.org/A066275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - n^(n-1) - (n-1)^n (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t
pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;          /* 1 of type seq_t */
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                   /* cast once */
    seq_t base_nm1 = (seq_t)(n - 1);          /* n-1 as seq_t (n>=1) */

    /* compute n^(n-1) */
    seq_t pow_n_minus1 = pow_seq_t(n_seq, n - 1);

    /* compute n^n  = n^(n-1) * n */
    seq_t pow_n = pow_n_minus1 * n_seq;

    /* compute (n-1)^n */
    seq_t pow_nm1_n = pow_seq_t(base_nm1, n);

    /* result = n^n - n^(n-1) - (n-1)^n */
    seq_t result = pow_n - pow_n_minus1;
    result -= pow_nm1_n;

    return result;
}
