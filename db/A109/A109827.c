/*
 * A109827  Numbers written in an alternating binary-then-ternary base.
 * https://oeis.org/A109827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the decimal integer whose digits form the mixed‑radix *
 *            representation of n using alternating radices 2,3,2,3,… (base‑2
 *            digit for * the least significant position, base‑3 for the next,
 *            etc.)
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 7775, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* the accumulated decimal number */
    seq_t mult   = 1;   /* current power of 10 (10^position) */
    idx_t  temp  = n;   /* working copy of the index */
    idx_t  pos   = 0;   /* digit position, starting from 0 (least significant) */

    while (temp > 0) {
        int base = (pos & 1) ? 3 : 2;       /* 2 for even pos, 3 for odd pos */
        seq_t digit = (seq_t)(temp % base); /* digit in this position (0,1, or 2) */
        result += digit * mult;             /* append digit to decimal result   */
        temp   /= base;                     /* move to next higher position     */
        mult   *= 10;                       /* next power of ten                */
        pos++;                              /* increment position counter       */
    }

    return result;
}
