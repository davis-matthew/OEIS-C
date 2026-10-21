/*
 * A215459  Arises in quick gossiping without duplicate transmission.
 * https://oeis.org/A215459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,2,4,8,12,16 for n =1..6; a(n) = 2n + 6 for n ≥ 7
 *            Implementation follows the OEIS definition: all even integers >=
 *            20 plus the first few special values
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first six terms */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)4;
        case 4: return (seq_t)8;
        case 5: return (seq_t)12;
        case 6: return (seq_t)16;
        default:
            /* For n ≥ 7: return 2n + 6 */
            return ((seq_t)n * (seq_t)2) + (seq_t)6;
    }
}
