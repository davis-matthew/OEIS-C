/*
 * A159481  Number of evil numbers <= n, see A001969.
 * https://oeis.org/A159481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} [popcount(k) is even] (Number of evil
 *            numbers ≤ n, i.e. numbers having an even number of 1‑bits.)
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int popcount_u64(uint64_t x)
{
    /* Count bits using the efficient "Brian Kernighan's algorithm". */
    int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Count evil numbers from 0 to n inclusive. */
    seq_t count = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* `i` is an idx_t (>= 64-bit).  Cast to uint64_t for bit
           counting; the cast is safe for a loop bound or small
           integer like this. */
        uint64_t u = (uint64_t)i;
        if ((popcount_u64(u) & 1) == 0) {
            count++;
        }
    }
    return count;
}
