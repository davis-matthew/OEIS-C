/*
 * A008873  3x+1 sequence starting at 97.
 * https://oeis.org/A008873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=97; a(n)= (a(n-1)%2==1)?3*a(n-1)+1:a(n-1)/2
 * Range:     u8 n <= 0, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 97;
    for (idx_t i = 0; i < n; ++i) {
        if (val & 1)
            val = val * 3 + 1;
        else
            val >>= 1;
    }
    return val;
}
