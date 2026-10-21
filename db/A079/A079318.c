/*
 * A079318  a(0) = 1; for n > 0, a(n) = (3^(A000120(n)-1) + 1)/2.
 * https://oeis.org/A079318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(A000120(n)-1)+1)/2
 * Range:     u8 n <= 126, u16 n <= 4094, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A079318.
   a(0) = 1
   For n > 0, a(n) = (3^(popcount(n)-1) + 1) / 2
   where popcount(n) is the number of 1‑bits in the binary
   representation of n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* compute popcount of n */
    unsigned int pc = 0;
    for (idx_t x = n; x; x >>= 1)
        if (x & 1)
            ++pc;

    /* compute 3^(pc-1) */
    seq_t p = 1;
    for (unsigned int i = 0; i < pc - 1; ++i)
        p *= 3;

    return (p + 1) / 2;
}
