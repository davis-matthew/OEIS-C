/*
 * A071584  Powers of 7 written backwards.
 * https://oeis.org/A071584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse_decimal(7^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: multiply by 7 repeatedly to obtain 7^n, then reverse its
   decimal digits using integer arithmetic.  This works for all supported
   seq_t types (fixed‑size and arbitrary‑precision). */
seq_t a(idx_t n)
{
    /* Compute 7^n by repeated multiplication. */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i)
        power = power * (seq_t)7;

    /* Reverse the decimal digits of power. */
    seq_t rev = 0;
    while (power != (seq_t)0)
    {
        seq_t digit = power % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        power = power / (seq_t)10;
    }
    return rev;
}
