/*
 * A124011  Add three, add six, add nine, ....
 * https://oeis.org/A124011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n-1)/2 + 5
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For safety, return 0 for
       n < 1, though the caller should not use such values. */
    if (n < 1)
        return 0;

    seq_t nn = (seq_t)n;
    /* Compute 3 * C(n, 2) + 5 where C(n, 2) = n*(n-1)/2.
       Dividing by 2 before the final multiplication reduces the
       risk of overflow in the intermediate product. */
    seq_t t;
    if ((nn & 1) == 0) /* nn even: (nn/2) * (nn-1) */
        t = (nn / 2) * (nn - 1);
    else              /* nn odd: nn * ((nn-1)/2) */
        t = nn * ((nn - 1) / 2);

    return 3 * t + 5;
}
