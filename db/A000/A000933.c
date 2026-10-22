/*
 * A000933  Genus of complete graph on n nodes.
 * https://oeis.org/A000933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   a(n) = ceiling((n-3)*(n-4)/12) for n >= 3, else 0
 * Range:     u8 n <= 19, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    
    seq_t nm3 = (seq_t)(n - 3);
    seq_t nm4 = (seq_t)(n - 4);
    seq_t product = nm3 * nm4;
    
    return (product + 11) / 12;
}
