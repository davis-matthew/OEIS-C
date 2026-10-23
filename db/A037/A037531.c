/*
 * A037531  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6 * a(n-1) + d_n,  with d_n = 1,1,2 repeating.
 * This is equivalent to interpreting the base‑6 digits d_1…d_n
 * (1,1,2,1,1,2,…) as the number a(n).
 */
seq_t a(idx_t n)
{
    /* The periodic digit pattern 1,1,2 */
    static const seq_t pattern[3] = { (seq_t)1, (seq_t)1, (seq_t)2 };

    if (n == 0)  /* below the offset – behaviour undefined but safe */
        return (seq_t)0;
    if (n == 1)  /* first term */
        return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t digit = pattern[(i-1) % 3];
        val = val * (seq_t)6 + digit;
    }
    return val;
}
