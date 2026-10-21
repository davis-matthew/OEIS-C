/*
 * A211770  a(n) = concatenation of periods of periodic sequences of ending
 *          digits of multiples of n.
 * https://oeis.org/A211770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n+10) for all n ≥ 1. The period of n modulo 10 gives
 *            the sequence of last digits of multiples of n. Concatenating
 *            that period yields the term
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 10.  The map n → n % 10 is well‑defined for all n ≥ 1. */
    idx_t m = n % 10;

    /* For multiples ending in 0 the period is just {0}.  Return 0 directly. */
    if (m == 0)
        return (seq_t)0;

    /* Start with the first last digit m and build the concatenated number. */
    seq_t digit = (seq_t)m;
    seq_t value = 0;

    do
    {
        /* Append the current digit to the result. */
        value = value * 10 + digit;

        /* Compute the next last digit of the next multiple of m. */
        digit = (seq_t)((digit + m) % 10);
    }
    /* Repeat until the first digit appears again, signalling the end of the period. */
    while (digit != (seq_t)m);

    return value;
}
