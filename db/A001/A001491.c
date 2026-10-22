/*
 * A001491  Opus numbers of Beethoven's nine symphonies.
 * https://oeis.org/A001491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Opus numbers of Beethoven's nine symphonies */

seq_t a(idx_t n)
{
    static const seq_t opus[] = {21, 36, 55, 60, 67, 68, 92, 93, 125};
    
    return opus[n - 1];
}
