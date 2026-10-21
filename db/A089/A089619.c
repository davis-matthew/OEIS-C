/*
 * A089619  a(n) = greatest prime factor of n^2 + (n+1)^2 for n >= 1.
 * https://oeis.org/A089619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of n^2 + (n+1)^2 = 2*n*(n+1) + 1
 * Range:     u8 n <= 10, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest prime factor of the integer N = 2*n*(n+1)+1.
   The computation uses only seq_t arithmetic, as required.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for out-of-bounds calls */

    /* Convert the index to the integer type used for the sequence. */
    seq_t nseq = (seq_t)n;

    /* Compute N = 2*n*(n+1) + 1 safely in seq_t. */
    seq_t t1 = nseq + 1;                      /* n + 1 */
    seq_t t2 = t1 * 2;                        /* 2*(n+1) */
    seq_t N  = nseq * t2 + 1;                 /* 2*n*(n+1) + 1 */

    /* Trial division to find the greatest prime divisor. */
    seq_t rem   = N;
    seq_t lpf   = 1;       /* greatest prime factor found so far */
    seq_t d     = 2;

    while (d * d <= rem) {
        if (rem % d == 0) {
            lpf = d;
            while (rem % d == 0) rem /= d;
        }
        d = (d == 2 ? 3 : d + 2);  /* after 2, skip even numbers */
    }

    if (rem > 1) lpf = rem;  /* remaining part is a prime larger than sqrt(N) */

    return lpf;
}
