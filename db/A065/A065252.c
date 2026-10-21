/*
 * A065252  The sequence A065251 reduced modulo 3 (i.e., replace every -1 with
 *          2).
 * https://oeis.org/A065252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(2n) = 2 - a(n), a(2n+1) = 2 - ((a(n)+1) mod 3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case (only needed for recursion on n == 0). */
    if (n == 0)
        return 0;

    /* Even or odd branch based on the least‑significant bit. */
    if ((n & 1) == 0) {
        seq_t half = a(n >> 1);        /* a(n/2) */
        return (seq_t)2 - half;        /* a(2n) = 2 - a(n/2) */
    } else {
        seq_t half = a(n >> 1);        /* a((2n+1)/2) = a(n/2) */
        seq_t mod = (half + 1) % (seq_t)3; /* (a(n/2)+1) mod 3 */
        return (seq_t)2 - mod;        /* a(2n+1) = 2 - ((a(n/2)+1) mod 3) */
    }
}
