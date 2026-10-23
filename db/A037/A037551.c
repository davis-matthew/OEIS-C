/*
 * A037551  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,2.
 * https://oeis.org/A037551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*10 + d(n) where d(1)=1, d(n)=1 if (n-1)%3==0 else
 *            2. This constructs the integer whose decimal expansion is the
 *            first n terms of the periodic sequence 1,2,2,1,2,2,... Indexing
 *            starts at n=1 as in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* guarantee n>=1; behavior below offset does not matter */
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        if (i == 1) {
            digit = 1;                 /* first digit always 1 */
        } else if (((i - 1) % 3) == 0) {
            digit = 1;                 /* positions 1,4,7,... after the first */
        } else {
            digit = 2;                 /* all other positions */
        }
        result = result * 10 + digit; /* append digit to the right */
    }
    return result;
}
