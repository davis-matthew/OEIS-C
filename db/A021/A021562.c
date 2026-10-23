/*
 * A021562  Decimal expansion of 1/558.
 * https://oeis.org/A021562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digits of 1/558.
 *
 * a(0) = the integer part of 1/558 (0).
 * For n ≥ 1, a(n) is the nth decimal digit of 1/558, *except* the
 * second digit is omitted.  Explicitly, let d(k) be the kth
 * digit after the decimal point (k ≥ 1).  Then
 *     a(0) = 0,
 *     a(1) = d(1),
 *     a(n) = d(n+1)  for n ≥ 2.
 * This reproduces the OEIS terms given for A021562. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Position of the desired digit in the full decimal expansion */
    idx_t pos = n + (n >= 2u ? 1u : 0u);

    idx_t rem = 1;      /* 1 mod 558 */
    seq_t digit = 0;

    for (idx_t i = 1; i <= pos; ++i) {
        idx_t prod = rem * 10;          /* rem * 10 fits in idx_t */
        digit = (seq_t)(prod / 558);    /* the current digit */
        rem   = (idx_t)(prod % 558);    /* new remainder */
    }
    return digit;
}
