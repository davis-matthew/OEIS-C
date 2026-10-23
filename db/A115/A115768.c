/*
 * A115768  Integers i such that 2*i XOR 5*i = 3*i, but 4*i XOR i is not 5*i.
 * https://oeis.org/A115768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115768: integers i such that
 *   (2*i) XOR (5*i) == 3*i   and   (4*i) XOR i != 5*i   */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t i = 0;

    while (1) {
        i += 1;                     /* examine successive positive integers */

        seq_t two   = (seq_t)2 * i;
        seq_t five  = (seq_t)5 * i;
        seq_t three = (seq_t)3 * i;

        if ((two ^ five) == three) {
            seq_t four = (seq_t)4 * i;
            if ((four ^ i) != five) {
                found += 1;
                if (found == n) {
                    return i;
                }
            }
        }
    }
}
