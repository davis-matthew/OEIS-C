/*
 * A027961  a(n) = Lucas(n+2) - 3.
 * https://oeis.org/A027961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3, with a(1)=1, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalent to a(n) = Lucas(n+2) - 3) */

seq_t a(idx_t n)
{
    /* handle n below offset – behaviour undefined, return 0 for safety */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = prev1 + prev2 + (seq_t)3;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
