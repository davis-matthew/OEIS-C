/*
 * A168280  a(n) = (1 - 5*(-1)^n + 10*n)/4.
 * https://oeis.org/A168280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 - 5*(-1)^n + 10*n)/4 (offset 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour for indices below offset is irrelevant */
        return 0;

    /* compute 10*n once; cast to seq_t to avoid mixing types */
    seq_t ten_n = (seq_t)10 * n;

    if (n & 1U)  /* odd n: (-1)^n = -1 */
        return (ten_n + (seq_t)6) / (seq_t)4;
    else         /* even n: (-1)^n = 1 */
        return (ten_n - (seq_t)4) / (seq_t)4;
}
