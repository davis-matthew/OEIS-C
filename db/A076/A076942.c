/*
 * A076942  Smallest k > 0 such that n*k+1 is a square.
 * https://oeis.org/A076942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that n*k + 1 is a square (returned
 *            value is k)
 * Range:     u8 n <= 17, u16 n <= 2505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Protect against a zero index; offset is 1 so it should not occur */
    if (n == 0) {
        return 0;
    }

    /* Search for the smallest integer s > 1 such that s² ≡ 1 (mod n).
       Since s = n−1 always satisfies the congruence, the loop will
       terminate before overflow occurs. */
    for (idx_t s = 2;; ++s) {
        seq_t sq  = (seq_t)s * (seq_t)s;
        seq_t diff = sq - 1;
        if (diff % (seq_t)n == 0) {
            /* k = (s² – 1) / n is the desired term */
            return diff / (seq_t)n;
        }
    }
}
