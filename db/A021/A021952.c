/*
 * A021952  Decimal expansion of 1/948.
 * https://oeis.org/A021952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 1/948. The expansion
 *            starts 0.00105348523206751…, with first two digits zero and a
 *            recurring period of 13 digits: 1,0,5,4,8,5,2,3,2,0,6,7,5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* first two digits are 0 */

    /* recurring 13‑digit cycle */
    static const seq_t cyc[13] = {
        (seq_t)1, (seq_t)0, (seq_t)5, (seq_t)4, (seq_t)8,
        (seq_t)5, (seq_t)2, (seq_t)3, (seq_t)2, (seq_t)0,
        (seq_t)6, (seq_t)7, (seq_t)5
    };

    return cyc[(n - 2) % 13];
}
