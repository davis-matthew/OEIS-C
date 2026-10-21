/*
 * A057168  Next larger integer with same binary weight (number of 1 bits) as
 *          n.
 * https://oeis.org/A057168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = the minimal m > n such that A000120(m) = A000120(n) (i.e.
 *            the next integer larger than n with the same number of 1‑bits)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is the index of the sequence, but the sequence works on the
       integer itself, so we cast it to the output type. */
    seq_t x = (seq_t)n;

    /* Handle the trivial case n = 0 (not part of the sequence) */
    if (x == (seq_t)0)
        return (seq_t)0;

    /* Compute c = lowest set bit of x = x & -x */
    seq_t c = x & (~x + 1);

    /* r = x + c : this moves the lowest set bit one position left */
    seq_t r = x + c;

    /* Construct the next higher number with the same popcount:
       ((r ^ x) >> 2) / c  replenishes the low bits */
    seq_t next = (((r ^ x) >> 2) / c) | r;

    return next;
}
