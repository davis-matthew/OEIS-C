/*
 * A208065  Number of n X 4 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 10 For n > 1: if n even: a(n) = n² · (n+8)² ÷ 4 if n odd
 *            : a(n) = (n−1) · (n+1) · (n+7) · (n+9) ÷ 4 The division is exact
 *            for all n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 10;

    if ((n & 1) == 0)          /* even n */
    {
        seq_t nn = (seq_t)n;
        seq_t t1 = nn * nn;                               // n²
        seq_t t2 = ((seq_t)n + 8) * ((seq_t)n + 8);        // (n+8)²
        return t1 * t2 / 4;
    }
    else                        /* odd n */
    {
        seq_t t1 = ((seq_t)n - 1) * ((seq_t)n + 1);        // (n−1)(n+1)
        seq_t t2 = ((seq_t)n + 7) * ((seq_t)n + 9);        // (n+7)(n+9)
        return t1 * t2 / 4;
    }
}
