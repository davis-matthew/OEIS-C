/*
 * A021983  Decimal expansion of 1/979.
 * https://oeis.org/A021983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/979.  a(n) is the nth digit after the decimal point. */
seq_t a(idx_t n)
{
    seq_t rem = (seq_t)1;                /* remainder before generating the digit */
    for (idx_t i = 0; i < n; ++i) {
        /* advance one digit: rem = (rem * 10) % 979 */
        rem = (seq_t)(((unsigned long long)rem * 10ULL) % 979ULL);
    }
    /* now produce the requested digit: digit = (rem * 10) / 979 */
    seq_t digit = (seq_t)(((unsigned long long)rem * 10ULL) / 979ULL);
    return digit;
}
