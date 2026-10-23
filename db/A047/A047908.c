/*
 * A047908  a(n+1) = a(n) op n, where op is successively +, *, -.
 * https://oeis.org/A047908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 26, u64 n <= 47, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047908:
   a(1) = 1;
   a(n+1) = a(n) + n   if n ≡ 1 (mod 3)
   a(n+1) = a(n) * n   if n ≡ 2 (mod 3)
   a(n+1) = a(n) - n   if n ≡ 0 (mod 3)
*/
seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    seq_t cur = (seq_t)1;
    for (idx_t i = (idx_t)1; i < n; ++i) {
        idx_t r = i % (idx_t)3;
        if (r == (idx_t)1) {
            cur += (seq_t)i;
        } else if (r == (idx_t)2) {
            cur *= (seq_t)i;
        } else {  /* r == 0 */
            cur -= (seq_t)i;
        }
    }
    return cur;
}
