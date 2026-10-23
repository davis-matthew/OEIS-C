/*
 * A028194  Expansion of 1/((1-5x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 120*a(n-2) + (8^(n+1) - 5^(n+1))/3, with
 *            a(0)=1, a(1)=35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)35;

    /* Two previous terms needed for the recurrence. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)35;  /* a(1) */

    /* Powers of 8 and 5 needed for the (8^(n+1)-5^(n+1))/3 term. */
    seq_t pow8 = (seq_t)512;  /* 8^3, used for n==2 */
    seq_t pow5 = (seq_t)125;  /* 5^3, used for n==2 */

    seq_t curr = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* (8^(i+1) - 5^(i+1)) / 3 */
        seq_t term = (pow8 - pow5) / (seq_t)3;

        curr = (seq_t)22 * prev1 - (seq_t)120 * prev2 + term;

        /* Shift previous values forward. */
        prev2 = prev1;
        prev1 = curr;

        /* Prepare next powers: 8^(i+2) and 5^(i+2). */
        pow8 *= (seq_t)8;
        pow5 *= (seq_t)5;
    }

    return prev1;
}
