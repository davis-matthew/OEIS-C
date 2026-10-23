/*
 * A008859  a(n) = Sum_{k=0..6} binomial(n,k).
 * https://oeis.org/A008859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..6} binomial(n, k) (binomial coefficient)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 122, u64 n <= 4867,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n == 0: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t sum = 1;      /* binomial(n,0) */
    seq_t c   = 1;      /* current binomial(n,k) value */

    /* Compute binomial(n,k) for k = 1 .. min(6,n) using recurrence
       c_k+1 = c_k * (n - k) / (k + 1)
       All intermediate multiplications are performed in a 128‑bit integer
       to avoid overflow when seq_t is 64‑bit. */
    idx_t limit = n < 6 ? n : 6;
    for (idx_t k = 0; k < limit; ++k) {
        /* Multiply c by (n - k) using 128‑bit arithmetic */
        __uint128_t temp = ( (__uint128_t)c ) * (uint64_t)(n - k);
        c = (seq_t)(temp / (k + 1));  /* exact division */
        sum += c;
    }

    return sum;
}
