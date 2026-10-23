/*
 * A018932  The number of permutations of n cards in which 4 will be the next
 *          hit after 2.
 * https://oeis.org/A018932
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)! – 3·(n-3)! + 2·(n-4)! for n > 5, with a(4) = 0 and
 *            a(5) = 3
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return (seq_t)0;
    if (n == 5)
        return (seq_t)3;

    /* Compute (n‑4)! ---------------------------------------------------- */
    idx_t m4 = n - 4;                /* m4 = n-4, ≥ 2 for n ≥ 6           */
    seq_t fact_n4 = (seq_t)1;
    for (idx_t i = 2; i <= m4; ++i)
        fact_n4 *= (seq_t)i;        /* fact_n4 = (n‑4)!                  */

    /* (n‑3)! and (n‑2)! are obtained by successive multiplications ------ */
    seq_t fact_n3 = fact_n4 * (seq_t)(m4 + 1);  /* (n‑3)! */
    seq_t fact_n2 = fact_n3 * (seq_t)(m4 + 2);  /* (n‑2)! */

    /* Final expression --------------------------------------------------- */
    seq_t result = fact_n2 - (seq_t)3 * fact_n3 + (seq_t)2 * fact_n4;
    return result;
}
