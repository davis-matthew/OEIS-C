/*
 * A154235  a(n) = ( (4 + sqrt(6))^n - (4 - sqrt(6))^n )/(2*sqrt(6)).
 * https://oeis.org/A154235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2) (a(0)=0, a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (so a(1) is the first term) */

seq_t a(idx_t n)
{
    /* Safety check for the offset – returns 0 for n < 1 (not expected) */
    if (n < 1)
        return (seq_t)0;

    /* Handle the first two terms explicitly */
    if (n == 1)
        return (seq_t)1;

    /* Iterate the linear recurrence up to the requested index.
       No caching is required – the cost is O(n) which is fine for
       n ≤ 10 000 and works with any seq_t (unsigned or big integer). */
    seq_t a_n_minus_2 = (seq_t)0;   /* a(0) */
    seq_t a_n_minus_1 = (seq_t)1;   /* a(1) */
    seq_t a_n = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 10*a(i-2) */
        a_n = (seq_t)(((seq_t)8 * a_n_minus_1) - ((seq_t)10 * a_n_minus_2));
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    return a_n_minus_1;  /* a(n) */
}
