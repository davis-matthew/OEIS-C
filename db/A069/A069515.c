/*
 * A069515  Number of transpositions (interchanges of adjacent digits,
 *          sometimes called inversions) needed to change all n-digit base 3
 *          numbers into nondecreasing order.
 * https://oeis.org/A069515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = (n-1)*(n+1)*3^(n-2) for n >= 2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Compute 3^(n-2) */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        power *= (seq_t)3;

    /* (n-1)*(n+1) fits in idx_t and can be promoted to seq_t safely */
    seq_t factor = (seq_t)(n - 1) * (seq_t)(n + 1);

    return factor * power;
}
