/*
 * A037654  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2.
 * https://oeis.org/A037654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3; a(n)=5*a(n-1)+digit(n), where digit(n) = 3,0,2
 *            repeating. Thus a(n) is the value in base 5 whose first n digits
 *            are the periodic sequence 3,0,2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A037654.  External linkage. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        switch ((i - 1) % 3) {
            case 0: digit = (seq_t)3; break;
            case 1: digit = (seq_t)0; break;
            case 2: digit = (seq_t)2; break;
        }
        result = result * (seq_t)5 + digit;
    }

    return result;
}
