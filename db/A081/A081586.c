/*
 * A081586  Fourth row of Pascal-(1,3,1) array A081578.
 * https://oeis.org/A081586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (32*n^3 - 24*n^2 + 28*n + 3)/3
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 738, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit arithmetic to avoid overflow on 64‑bit builds. */
    unsigned __int128 nn = n;                 /* promote index to 128 bits   */
    unsigned __int128 num = (32 * nn * nn * nn) /* 32·n³ */
                       - (24 * nn * nn)        /*   24·n² */
                       + (28 * nn)             /*    28·n  */
                       + 3;                    /*        +3  */
    return (seq_t)(num / 3);                 /* final integer division      */
}
