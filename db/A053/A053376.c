/*
 * A053376  a(n) contains n digits (either '1' or '8') and is divisible by
 *          2^n.
 * https://oeis.org/A053376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 8;
   For n > 1: a(n) = a(n-1) + 10^(n-1) * (8 - 7 * ((a(n-1) >> (n-1)) & 1)) */
seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)8;
    if (n == 1)
        return a_val;

    seq_t pow10 = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        /* 10^(i-1) */
        pow10 *= (seq_t)10;

        /* parity of (a(n-1) / 2^(n-1)) */
        seq_t q = a_val >> (i - 1);
        seq_t digit = ((q & (seq_t)1) == (seq_t)0) ? (seq_t)8 : (seq_t)1;

        a_val += digit * pow10;
    }
    return a_val;
}
