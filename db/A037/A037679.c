/*
 * A037679  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1}^{n} d_k * 9^{n-k} where the digits d_k repeat
 *            the pattern 1,0,2,3. This generates the base‑9 number consisting
 *            of the first n terms of that periodic sequence
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset is 1, so this case is irrelevant */

    static const unsigned pattern[4] = {1, 0, 2, 3};
    seq_t result = 0;
    idx_t i;

    for (i = 1; i <= n; ++i)
    {
        result = result * 9 + (seq_t)pattern[(i - 1) & 3];
    }

    return result;
}
