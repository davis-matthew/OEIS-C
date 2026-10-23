/*
 * A117448  Diagonal sums of number triangle A117446.
 * https://oeis.org/A117448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(L(k/3), n-2k) where L(j/p) is the
 *            Legendre symbol of j and the prime p (=3)
 * Range:     i8 n <= 281, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the k-th term of sequence A117448. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 2;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t m = (seq_t)n - (seq_t)2 * (seq_t)k;
        int r = (int)(k % 3);          /* remainder of k mod 3 */
        seq_t term = 0;
        switch (r) {
            case 0: /* Legendre symbol L = 0 */
                if (m == 0) term = 1;
                break;
            case 1: /* Legendre symbol L = 1 */
                if (m == 0 || m == 1) term = 1;
                break;
            case 2: /* Legendre symbol L = -1 */
                term = ((m & 1) == 0) ? 1 : -1;
                break;
        }
        sum += term;
    }
    return sum;
}
