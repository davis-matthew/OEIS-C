/*
 * A021197  Decimal expansion of 1/193.
 * https://oeis.org/A021197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th decimal digit of 1/193 by long division. */
seq_t a(idx_t n)
{
    /* 1/193 expansion: digits after the decimal point. */
    static const seq_t divisor = 193;
    seq_t rem = 1;          /* current remainder (initially the numerator). */
    seq_t digit = 0;        /* value to return. */
    /* Perform long division n+1 times to reach a(n). */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod = rem * 10;   /* multiply remainder by 10 (bring down next digit). */
        digit = prod / divisor;  /* next decimal digit. */
        rem = prod % divisor;    /* new remainder. */
    }
    return digit;
}
