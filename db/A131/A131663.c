/*
 * A131663  Numbers k such that either k or k+1 is divisible by the numbers
 *          from 1 to 10.
 * https://oeis.org/A131663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 2520 for n >= 5; a(1)=720, a(2)=1799, a(3)=2519,
 *            a(4)=2520
 * Range:     u8 none, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, just return 0 */

    /* Base values for the four residue classes modulo 4 */
    static const seq_t base[5] = {0, 720, 1799, 2519, 2520};

    idx_t rem = n % 4ULL;          /* remainder in [0,3] */
    if (rem == 0) rem = 4;        /* treat residue 0 as 4 for indexing */

    idx_t k = (n - rem) / 4;       /* number of full 4-step cycles before n */

    return base[rem] + (seq_t)2520 * (seq_t)k;
}
