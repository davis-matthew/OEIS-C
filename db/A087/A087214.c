/*
 * A087214  Expansion of e.g.f.: exp(x)/(1-x^2/2).
 * https://oeis.org/A087214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} n! / ((n-2k)! * 2^k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity a_k = a_{k-1} * (n-2k+2)*(n-2k+1)/2,   */
/* with an early division by 2 to avoid overflow on fixed‑size seq_t.   */

seq_t a(idx_t n)
{
    seq_t sum  = 1;          /* a(0) = 1 */
    if (n < 2)               /* a(1) = 1 */
        return sum;

    seq_t term = 1;          /* term for k = 0 */
    idx_t kmax = n / 2;      /* floor(n/2) */

    for (idx_t k = 1; k <= kmax; ++k) {
        /* Factors for the recurrence */
        seq_t a1 = (seq_t)(n - 2*k + 2); /* (n-2k+2)  */
        seq_t a2 = (seq_t)(n - 2*k + 1); /* (n-2k+1)  */

        /* One of a1 or a2 is even – divide that one by 2 first */
        if ((a1 & 1) == 0)
            a1 /= 2;
        else
            a2 /= 2;

        /* Update the term: term *= a1 * a2   (no overflow on fixed size) */
        term *= a1;
        term *= a2;

        /* Accumulate the sum */
        sum += term;
    }
    return sum;
}
