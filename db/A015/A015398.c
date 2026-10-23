/*
 * A015398  Gaussian binomial coefficient [ n,10 ] for q=-10.
 * https://oeis.org/A015398
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1}^{10} ((-10)^{n-i+1} – 1) / ((-10)^i – 1)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Gaussian binomial coefficient [ n,10 ] at q = –10.
   The function is defined for all n ≥ 10; for n  < 10 it returns 0.  */
seq_t a(idx_t n)
{
    if (n < 10)          /* undefined for the minimal indices */
        return 0;

    seq_t res = 1;        /* a(10) */

    /* 10^10 as the base for forward powers */
    seq_t pow10k = 1;
    for (idx_t k = 0; k < 10; ++k)
        pow10k *= 10;

    /* start with 10^10, then grow to 10^n */
    seq_t pow_i = pow10k;

    for (idx_t i = 11; i <= n; ++i) {
        /* compute 10^i  and 10^{i-10} */
        pow_i *= 10;                     /* 10^i */
        seq_t pow_d = pow_i / pow10k;    /* 10^{i-10} */

        /* |(-10)^i – 1|  */
        seq_t num = (i & 1) ? (pow_i + 1) : (pow_i - 1);

        /* |(-10)^{i-10} – 1|  */
        seq_t den = ((i - 10) & 1) ? (pow_d + 1) : (pow_d - 1);

        /* a(i) = a(i-1) * num / den   (exact division) */
        seq_t temp = res * num;
        res      = temp / den;
    }

    return res;
}
