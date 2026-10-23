/*
 * A117967  Positive part of inverse of A117966; write n in balanced ternary
 *          and then replace (-1)'s with 2's.
 * https://oeis.org/A117967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(0) = 0, a(3n) = 3a(n), a(3n+1) = 3a(n)+1, a(3n-1) = 3a(n)+2
 * Range:     u8 n <= 121, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a_rec(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    idx_t r = n % (idx_t)3;
    if (r == 0) {
        return a_rec(n / (idx_t)3) * (seq_t)3;
    } else if (r == 1) {
        return a_rec(n / (idx_t)3) * (seq_t)3 + (seq_t)1;
    } else {          /* r == 2, i.e. n = 3k-1 */
        return a_rec((n + (idx_t)1) / (idx_t)3) * (seq_t)3 + (seq_t)2;
    }
}

seq_t a(idx_t n) {
    return a_rec(n);
}
