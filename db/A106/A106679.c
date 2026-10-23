/*
 * A106679  Marks on 14-mark optimal Golomb ruler.
 * https://oeis.org/A106679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   This function returns the position of the n‑th mark on the *
 *            optimal 14‑mark Golomb ruler (known minimal length 127). For n
 *            >= 14 * no mark is defined; the function returns 0
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The 14 optimal marks are fixed by the definition of the sequence. */
    static const seq_t marks[14] = {
        0,  5,  28, 38, 41, 49, 50,
        68, 75, 92, 107, 121, 123, 127
    };

    if (n < 14) return marks[n];
    return 0;                 /* out of range value */
}
