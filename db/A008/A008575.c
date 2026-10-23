/*
 * A008575  Sectors on dartboard.
 * https://oeis.org/A008575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1..20) are the numbers of a standard dartboard written
 *            clockwise starting from 1; a(21)=25 (outer bull), a(22)=50
 *            (inner bull). For n>22 the sequence is defined as 0
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The board has 22 sectors: 20 numbered wedges and 2 bulls.  The
       correct order (starting at sector 1 and going clockwise) is:
       1,18,4,13,6,10,15,2,17,3,19,7,16,8,11,14,9,12,5,20,25,50.
       The array below contains this fixed order.  */
    static const seq_t order[22] = {
        (seq_t)1,  (seq_t)18, (seq_t)4,  (seq_t)13, (seq_t)6,
        (seq_t)10, (seq_t)15, (seq_t)2,  (seq_t)17, (seq_t)3,
        (seq_t)19, (seq_t)7,  (seq_t)16, (seq_t)8,  (seq_t)11,
        (seq_t)14, (seq_t)9,  (seq_t)12, (seq_t)5,  (seq_t)20,
        (seq_t)25, (seq_t)50
    };

    if (n == 0)  /* n<1 is outside the defined offset */
        return 0;
    if (n <= 22)
        return order[n - 1];
    return 0;    /* undefined beyond the 22 sectors */
}
