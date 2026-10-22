/*
 * A006499  Number of restricted circular combinations.
 * https://oeis.org/A006499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(floor(n/2+1))*Lucas(ceiling(n/2+1)), with Lucas(0)
 *            = 2, Lucas(1) = 1, Lucas(k) = Lucas(k-1) + Lucas(k-2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k1 = n / 2 + 1;
    idx_t k2 = (n + 1) / 2 + 1;
    seq_t x = 2, y = 1; /* x = Lucas(i), y = Lucas(i+1) */
    seq_t l1 = 0, l2 = 0;
    for (idx_t i = 0; i <= k2; i++) {
        if (i == k1) l1 = x;
        if (i == k2) l2 = x;
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return l1 * l2;
}
