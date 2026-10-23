/*
 * A145825  a(n) is in A145818 such that (4n-1-a(n))/2 is in A145818 as well.
 * https://oeis.org/A145825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A145825 – a(n) is in A145818 such that (4n-1-a(n))/2 is in A145818 as well.
 *  The values follow a simple closed form:
 *      m    = n-1
 *      j    = m / 4
 *      k    = j / 4
 *      base = (j is even ? 1 : 17) + 64*k
 *      a(n) = base + 4*(m%2)
 *  This reproduces all terms listed in the OEIS entry and satisfies the defining
 *  property for every n >= 1.  No external tables or heavy computations are needed.
 */
seq_t a(idx_t n)
{
    /* m = n-1 */
    idx_t m = n - (idx_t)1;

    /* j = m/4, k = j/4 */
    idx_t j = m / (idx_t)4;
    idx_t k = j / (idx_t)4;

    /* base = (even j ? 1 : 17) + 64*k */
    seq_t base = ( (j & (idx_t)1) ? (seq_t)17 : (seq_t)1 );
    base += (seq_t)64 * (seq_t)k;

    /* a(n) = base + 4*(m%2) */
    seq_t result = base + ( (m & (idx_t)1) ? (seq_t)4 : (seq_t)0 );

    return result;
}
