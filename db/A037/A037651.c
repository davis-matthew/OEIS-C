/*
 * A037651  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3 Recurrence: a(n) = 9 * a(n-1) + d_{n-1} where d_k = 3
 *            if k mod 3 = 0, 0 if k mod 3 = 1, 1 if k mod 3 = 2 (Periodic
 *            digits 3,0,1 in base 9)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* zero or negative indices are outside the defined offset */
    if (n == 0)
        return 0;

    /* a(1) = 3 */
    seq_t result = (seq_t)3;

    /* Build the number by repeatedly appending the next digit
       of the periodic sequence to the right (multiply by 9,
       add the digit). */
    for (idx_t i = 1; i < n; ++i) {
        seq_t d;
        if (i % 3 == 0)
            d = (seq_t)3;
        else if (i % 3 == 1)
            d = (seq_t)0;
        else
            d = (seq_t)1;
        result = result * 9 + d;
    }

    return result;
}
