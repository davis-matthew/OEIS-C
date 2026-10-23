/*
 * A022270  a(n) = n*(13*n - 1)/2.
 * https://oeis.org/A022270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n - 1)/2
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Perform calculation in a wider unsigned integer to avoid overflow
       when the intermediate value exceeds the range of seq_t.
    */
    unsigned __int128 nn = n;
    unsigned __int128 val = nn * ((unsigned __int128)13 * nn - 1);
    val /= 2;
    return (seq_t)val;
}
