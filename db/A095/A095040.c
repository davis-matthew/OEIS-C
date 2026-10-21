/*
 * A095040  An example of a (v,k,lambda)=(183,14,1) cyclic difference set.
 * https://oeis.org/A095040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = D[(n-1) mod 14] + 183 * ((n-1) / 14) where D =
 *            {1,13,20,21,23,44,61,72,77,86,90,116,122,169}
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t D[14] = {
        1, 13, 20, 21, 23, 44, 61, 72, 77, 86,
        90, 116, 122, 169
    };
    idx_t m = n - 1;          /* zero‑based index */
    idx_t i = m % 14;         /* position within the block */
    idx_t k = m / 14;         /* number of completed blocks */
    return D[i] + (seq_t)k * (seq_t)183;
}
