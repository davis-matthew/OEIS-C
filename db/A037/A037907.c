/*
 * A037907  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*4^i is the base-4 representation of n.
 * https://oeis.org/A037907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ i | 1 ≤ i ≤ m, |d(i) - d(i-1)| = 1 }|, where n =
 *            Σ_{k=0}^{m} d(k)·4^k is the base‑4 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)
        return count;            /* undefined for offset, but harmless */

    unsigned last = (unsigned)(n & 0x3);
    n >>= 2;

    while (n) {
        unsigned d = (unsigned)(n & 0x3);
        unsigned diff = (last > d) ? (last - d) : (d - last);
        if (diff == 1)
            count++;
        last = d;
        n >>= 2;
    }

    return count;
}
