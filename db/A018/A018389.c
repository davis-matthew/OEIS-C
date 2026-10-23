/*
 * A018389  Divisors of 316.
 * https://oeis.org/A018389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 316 in ascending order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No need to handle n==0 – the definition starts at offset 1 */
    if (n <= 0)
        return 0;

    seq_t divs[6];
    int k = 0;
    for (int e2 = 0; e2 <= 2; ++e2)          /* 2^0, 2^1, 2^2 */
    {
        seq_t pow2 = ((seq_t)1 << e2);
        for (int e79 = 0; e79 <= 1; ++e79)   /* 79^0, 79^1 */
        {
            seq_t pow79 = e79 ? (seq_t)79 : (seq_t)1;
            divs[k++] = pow2 * pow79;
        }
    }

    /* Sort the 6 divisors (simple insertion sort, trivial size) */
    for (int i = 1; i < 6; ++i)
        for (int j = i; j > 0 && divs[j] < divs[j - 1]; --j)
        {
            seq_t tmp = divs[j];
            divs[j] = divs[j - 1];
            divs[j - 1] = tmp;
        }

    /* Return the nth divisor if it exists; otherwise return 0 */
    if ((idx_t)n <= 6u)
        return divs[n - 1];
    else
        return 0;
}
