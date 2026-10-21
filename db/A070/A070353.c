/*
 * A070353  a(n) = 3^n mod 14.
 * https://oeis.org/A070353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 14
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The values repeat with period 6: 1,3,9,13,11,5.  */
    static const seq_t period[6] = {
        (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)13, (seq_t)11, (seq_t)5
    };
    return period[n % 6];
}
