/*
 * A124769  Number of strictly decreasing runs for compositions in standard
 *          order.
 * https://oeis.org/A124769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(0)=0; for n>0, a(n) is the number of strictly decreasing runs
 *            in the composition whose index is n in the standard order of
 *            compositions (A066099). Compositions are constructed from the
 *            binary representation of n as described in the OEIS entry
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Build the list of prefix positions (indices) from the binary
       representation of n.  The first position is always 0. */
    idx_t idx[65];          /* enough for 64‑bit indices */
    idx[0] = 0;
    int  t = 1;             /* number of positions stored */
    idx_t k   = n;
    idx_t bit = 0;
    while (k) {
        if (k & 1)          /* set bit at position 'bit' */
            idx[t++] = bit + 1;
        k >>= 1;
        ++bit;
    }

    /* The composition parts are the differences between consecutive
       positions, taken in reverse order. */
    seq_t runs = 1;                 /* at least one run */
    int   last_part = (int)(idx[t-1] - idx[t-2]); /* first part */

    for (int i = t-2; i >= 1; --i) {
        int part = (int)(idx[i] - idx[i-1]);
        if (last_part <= part)    /* new run when previous <= current */
            ++runs;
        last_part = part;
    }

    return runs;
}
