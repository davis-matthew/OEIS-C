/*
 * A139355  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives max{e(n), o(n)}.
 * https://oeis.org/A139355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max(e(n), o(n)) where e(n) = # of 1‑bits in even
 *            positions (0‑based from the least significant bit) and o(n) = #
 *            of 1‑bits in odd positions. Since e(n)+o(n)=popcount(n), a(n) is
 *            the greater of these two counts
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count set bits of x using the Kernighan algorithm. */
static int popcount_u64(idx_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Masks to isolate bits in even and odd positions.  */
    const idx_t even_mask = 0x5555555555555555ULL; /* 0101... binary */
    const idx_t odd_mask  = 0xAAAAAAAAAAAAAAAAULL; /* 1010... binary */

    int e = popcount_u64(n & even_mask);
    int o = popcount_u64(n & odd_mask);

    return (seq_t)((e > o) ? e : o);
}
