/*
 * A116922  a(n) = smallest integer >= n/2 which is coprime to n.
 * https://oeis.org/A116922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest integer >= n/2 that is coprime to n (closed
 *            form: if n is odd, a(n) = (n+1)/2; if n ≡ 0 (mod 4), a(n) = n/2
 *            + 1; if n ≡ 2 (mod 4), a(n) = n/2 + 2; a(1) = a(2) = 1)
 * Range:     u8 n <= 441, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)1;
    }
    if (n & 1ULL) {           /* n is odd */
        return ((seq_t)n + 1) / 2;
    }
    seq_t half = (seq_t)n / 2;    /* n is even, so division is exact */
    if ((n & 3ULL) == 0) {        /* n ≡ 0 (mod 4) */
        return half + 1;
    }
    /* n ≡ 2 (mod 4) */
    return half + 2;
}
