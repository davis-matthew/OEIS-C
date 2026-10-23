/*
 * A009925  Coordination sequence for CaF2(2), F position.
 * https://oeis.org/A009925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*(-1)^n*n^2+15*n^2+(-1)^n+7)/4 for n>0; a(0)=1
 * Range:     u8 n <= 7, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use 128‑bit intermediate to avoid overflow when seq_t is 64 bits. */
    __int128 nn = (__int128)n * (__int128)n;  /* nn = n^2 */

    if ((n & 1) == 0)          /* n even */
        return (seq_t)(5 * nn + 2);
    else                       /* n odd */
        return (seq_t)((5 * nn + 3) / 2);
}
