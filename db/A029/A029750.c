/*
 * A029750  List of numbers of the form 2^k times 1, 3, 5 or 7.
 * https://oeis.org/A029750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) for n > 8; a(1)=1, a(2)=2, a(3)=3, a(4)=4,
 *            a(5)=5, a(6)=6, a(7)=7, a(8)=8
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 123, u64 n <= 251,
 *            u128 n <= 507, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    if (n <= 8) {
        return base[n - 1];
    }
    /* n > 8 */
    idx_t k = (n - 5) / 4;          /* number of factors of 2 */
    idx_t m = n - 4 * k;            /* position in the base array (1‑based) */
    seq_t val = base[m - 1];
    for (idx_t i = 0; i < k; ++i) {
        val = val << 1;             /* multiply by 2 */
    }
    return val;
}
