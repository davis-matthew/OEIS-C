/*
 * A040536  Continued fraction for sqrt(560).
 * https://oeis.org/A040536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n>0, a(n)=46 if (n-1)%4==3 else 1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    if (((n - 1) % 4) == 3)
        return (seq_t)46;
    else
        return (seq_t)1;
}
