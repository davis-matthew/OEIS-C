/*
 * A021269  Decimal expansion of 1/265.
 * https://oeis.org/A021269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/265 at position n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 13‑digit repeating period of the expansion of 1/265 after the two
       leading zeros (offset 0).  The digits are:
           3,7,7,3,5,8,4,9,0,5,6,6,0
    */
    static const seq_t period[13] = {
        (seq_t)3, (seq_t)7, (seq_t)7, (seq_t)3, (seq_t)5, (seq_t)8,
        (seq_t)4, (seq_t)9, (seq_t)0, (seq_t)5, (seq_t)6, (seq_t)6,
        (seq_t)0
    };

    if (n < 2)
        return 0;

    idx_t idx = (n - 2) % 13;
    return period[idx];
}
