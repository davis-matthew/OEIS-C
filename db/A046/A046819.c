/*
 * A046819  Number of 1's in binary expansion of 3n+2.
 * https://oeis.org/A046819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1's in the binary representation of (3*n + 2)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t val = (idx_t)3 * n + 2;      /* compute 3*n + 2 in 64‑bit arithmetic */
    seq_t cnt = 0;                     /* count of set bits */
    while (val) {                      /* Brian Kernighan's algorithm */
        val &= val - 1;
        ++cnt;
    }
    return cnt;                         /* return as seq_t */
}
