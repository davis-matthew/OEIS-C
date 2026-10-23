/*
 * A156887  a(n) = Sum_{k=0..n} C(n,k) * C(4*n+k,k).
 * https://oeis.org/A156887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C(4*n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A156887.  The function
   iteratively updates the two binomial coefficients
   C(n,k) and C(4*n+k,k) to avoid recomputing them from scratch
   at each step.  All arithmetic is performed in the build‑defined
   arbitrary‑precision type seq_t. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;   /* C(n,0)   */
    seq_t b   = 1;   /* C(4*n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update C(n,k)   = C(n,k-1) * (n-k+1)/k   */
            c = (c * ((seq_t)n - (seq_t)k + 1)) / (seq_t)k;

            /* Update C(4*n+k,k) = C(4*n+k-1,k-1) *
                                   (4*n+k)/k           */
            b = (b * (((seq_t)4 * (seq_t)n) + (seq_t)k)) / (seq_t)k;
        }

        sum += c * b;
    }

    return sum;
}
