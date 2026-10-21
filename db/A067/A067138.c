/*
 * A067138  OR-numbral multiplication table, read by antidiagonals.
 * https://oeis.org/A067138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look, changed
 * Formula:   a(n) is the element at index n of the OR‑numbral multiplication
 *            table T(n,m) read by antidiagonals. T(n,m) is defined as the
 *            bitwise OR of all shifts of n by the set bit positions of m:
 *            T(n,m) = OR_{k: (m>>k)&1} (n << k) This satisfies the
 *            properties: T(n,0)=0, T(n,1)=n, and T(n,2^k)=n*2^k. a(n)...
 * Range:     u8 n <= 543, u16 n <= 9963, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal d such that the first d+1 diagonal entries
       (of sizes 1+2+…+(d+1)) exceed n. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Number of elements before this diagonal. */
    idx_t prev = (d * (d + 1)) / 2;

    /* Position within the diagonal: i decreases from d to 0. */
    idx_t i = n - prev;          /* row index */
    idx_t j = d - i;             /* column index */

    /* Compute T(i,j) = OR of shifted i over set bits of j. */
    seq_t res = 0;
    seq_t shift = (seq_t)i;
    idx_t bits = j;
    while (bits)
    {
        if (bits & 1)
            res |= shift;
        shift <<= 1;
        bits >>= 1;
    }
    return res;
}
