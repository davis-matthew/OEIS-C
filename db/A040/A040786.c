/*
 * A040786  Continued fraction for sqrt(815).
 * https://oeis.org/A040786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28; for n>0, a(n)=period[(n-1)%12], where period =
 *            {1,1,4,1,2,5,2,1,4,1,1,56}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t period[12] = {1,1,4,1,2,5,2,1,4,1,1,56};
    if (n == 0) return 28;
    return period[(n - 1) % 12];
}
