/*
 * A100814  Digits 9 to 0 are written in order with increasing number of
 *          digits for each member of the sequence. Leading zeros are counted,
 *          but are not written down.
 * https://oeis.org/A100814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = integer formed by concatenating n consecutive digits of
 *            the infinite repeating sequence 9,8,7,6,5,4,3,2,1,0 starting at
 *            position ((n-1)*n)/2 + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; behaviour for 0 is undefined */
    idx_t start_idx = ((idx_t)(n - 1) * n) / 2 + 1;
    seq_t start = (seq_t)start_idx;
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t pos   = start + (seq_t)i;
        seq_t digit = 9 - ((pos - 1) % 10);
        result = result * 10 + digit;
    }
    return result;
}
