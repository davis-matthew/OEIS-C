/*
 * A055956  n + reversal of base 8 digits of n (written in base 10).
 * https://oeis.org/A055956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_base8(n)
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9528 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t
reverse_base8(idx_t n)
{
    seq_t rev = 0;          /* accumulator for the reversed number      */
    idx_t  tmp = n;         /* temporary copy of the index             */
    while (tmp != 0) {
        rev = rev * 8 + (tmp & 7); /* rev = rev*8 + lowest octal digit */
        tmp >>= 3;                    /* shift right by one octal digit   */
    }
    return rev;
}
/* Computes the sequence A055956: n + reverse of the octal digits of n,
   where the reverse is interpreted as a base‑10 integer.  */

seq_t
a(idx_t n)
{
    seq_t rev = reverse_base8(n);           /* reversed octal value      */
    return (seq_t)n + rev;                  /* n + reversed value        */
}
