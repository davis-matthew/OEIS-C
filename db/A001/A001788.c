/*
 * A001788  a(n) = n*(n+1)*2^(n-2).
 * https://oeis.org/A001788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t nn = (seq_t)n;
    seq_t result = nn * (nn + 1);
    result = result << (n - 2);
    return result;
}
