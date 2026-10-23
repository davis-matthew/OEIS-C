/*
 * A040521  Continued fraction for sqrt(545).
 * https://oeis.org/A040521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23; for n≥1 the terms repeat with period 6:
 *            a(1)=2,a(2)=1,a(3)=8,a(4)=1,a(5)=2,a(6)=46, and thereafter
 *            a(n+6)=a(n+1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic block after the initial term */
    static const seq_t block[6] = {2, 1, 8, 1, 2, 46};

    if (n == 0) {
        return 23;
    } else {
        /* (n-1) is safe because n>0 here */
        return block[(n - 1) % 6];
    }
}
