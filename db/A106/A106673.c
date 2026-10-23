/*
 * A106673  Marks on lexicographically third 7-mark optimal Golomb ruler.
 * https://oeis.org/A106673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106673: Marks on lexicographically third 7‑mark optimal Golomb ruler.
   The sequence consists of the seven mark positions of this ruler:
   0, 1, 11, 16, 19, 23, 25.  For indices beyond 6 the value is undefined,
   so the function returns 0. */

seq_t a(idx_t n)
{
    /* Static table of the seven marks.  The type seq_t will automatically
       promote the integer literals as required by the build environment. */
    static const seq_t marks[7] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)11,
        (seq_t)16,
        (seq_t)19,
        (seq_t)23,
        (seq_t)25
    };

    return (n < 7) ? marks[(idx_t)n] : (seq_t)0;
}
