/*
 * A025989  Expansion of 1/((1-2*x)*(1-5*x)*(1-6*x)*(1-10*x)).
 * https://oeis.org/A025989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(n+1)-2^(n+1))/3 + 16*a(n-1) - 60*a(n-2); a(0) = 1,
 *            a(1) = 23
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;

    seq_t pow2 = (seq_t)1;       /* 2^0 */
    seq_t pow5 = (seq_t)1;       /* 5^0 */

    seq_t a_prev2 = (seq_t)1;    /* a(0) */
    seq_t a_prev1 = (seq_t)23;   /* a(1) */
    seq_t a_curr  = 0;

    /* iterate over j = 1 .. n+1, where j = i+1 for the recurrence */
    for (idx_t j = 1; j <= n + 1; ++j) {
        /* update powers to 2^j and 5^j */
        pow2 *= (seq_t)2;
        pow5 *= (seq_t)5;

        idx_t i = j - 1;
        if (i >= 2) {
            seq_t term = (pow5 - pow2) / (seq_t)3;
            a_curr = term + a_prev1 * (seq_t)16 - a_prev2 * (seq_t)60;
            if (i == n)
                return a_curr;

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
    }

    /* unreachable for n >= 0 */
    return (seq_t)0;
}
