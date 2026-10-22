/*
 * A007911  a(n) = (n-1)!! - (n-2)!!.
 * https://oeis.org/A007911
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)!! - (n-2)!!
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 34, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t d1 = 1; /* (n-1)!! */
    seq_t d2 = 1; /* (n-2)!! */
    idx_t k;
    for (k = 1; k <= n - 1; k++) {
        if ((k & 1) == ((n - 1) & 1))
            d1 *= (seq_t)k;
        else
            d2 *= (seq_t)k;
    }
    return d1 - d2;
}
