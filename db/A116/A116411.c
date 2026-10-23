/*
 * A116411  Coefficient of x^n in the expansion of (1+x+x^3)^n.
 * https://oeis.org/A116411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/3⌋} binom(n,3k) * binom(3k,k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t C_n = 1;      /* binom(n,0) */
    seq_t C_3k_k = 1;   /* binom(0,0) */
    const idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        if (k > 0)
        {
            /* Update binom(n,3k) from binom(n,3(k-1)) using a 3‑step recurrence */
            idx_t m = 3 * k;
            seq_t temp = C_n;

            seq_t num1 = (seq_t)(n - (m - 3));
            seq_t den1 = (seq_t)(m - 2);
            temp = temp * num1 / den1;

            seq_t num2 = (seq_t)(n - (m - 2));
            seq_t den2 = (seq_t)(m - 1);
            temp = temp * num2 / den2;

            seq_t num3 = (seq_t)(n - (m - 1));
            seq_t den3 = (seq_t)m;
            temp = temp * num3 / den3;

            C_n = temp;

            /* Update binom(3k,k) from binom(3(k-1),k-1) */
            seq_t num_a = (seq_t)(3 * k);        /* 3k            */
            seq_t num_b = (seq_t)(3 * k - 1);    /* 3k-1          */
            seq_t num_c = (seq_t)(3 * k - 2);    /* 3k-2          */
            seq_t denom  = (seq_t)2 * (seq_t)k * (seq_t)k * (seq_t)(2 * k - 1);
            temp = C_3k_k * num_a * num_b * num_c;
            temp /= denom;
            C_3k_k = temp;
        }

        sum += C_n * C_3k_k;
    }

    return sum;
}
