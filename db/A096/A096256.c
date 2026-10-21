/*
 * A096256  Decimal expansion of the probability of rolling a Yahtzee (5 of a
 *          kind) in the dice game Yahtzee.
 * https://oeis.org/A096256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096256 – Decimal expansion of 347897/7558272, skipping the first
    zero after the decimal point (as in the OEIS listing).
    a(n) = floor( (347897/7558272) * 10^(n+1) ) mod 10
    (for n = 0 gives the integer part, n = 1 gives the first non‑zero
    decimal digit).  The computation uses the standard long‑division
    algorithm: perform n+1 successive 10× multiplications and divisions
    by the denominator.  */

seq_t a(idx_t n)
{
    const seq_t NUM = 347897;
    const seq_t DEN = 7558272;

    /* The integer part (n = 0).  */
    if (n == 0)
        return 0;

    seq_t rem   = NUM;   /* current remainder, always < DEN */
    seq_t digit = 0;     /* last computed decimal digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * 10;
        digit = rem / DEN;
        rem   = rem % DEN;
    }

    /* digit now holds a(n) */
    return digit;
}
