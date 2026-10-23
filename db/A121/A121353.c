/*
 * A121353  a(n) = (3*n - 2)*a(n-1) - a(n-2) starting a(0)=0, a(1)=1.
 * https://oeis.org/A121353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 2)*a(n-1) - a(n-2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute the sequence iteratively up to n. */
    seq_t a_prev2 = (seq_t)0; /* a(0) */
    seq_t a_prev1 = (seq_t)1; /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* (3*i - 2) fits in idx_t; cast to seq_t for multiplication. */
        a_curr = (seq_t)(3 * i - 2) * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
