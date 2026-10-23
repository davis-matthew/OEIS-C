/*
 * A116734  Number of permutations of length n which avoid the patterns 231,
 *          1432, 4123.
 * https://oeis.org/A116734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2*n - 4 with a(1)=1, a(2)=2,
 *            a(3)=5 (OEIS A116734)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle values below the offset; any behaviour is allowed */
    if (n == 0)
        return (seq_t)0;

    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* For n >= 4 compute iteratively using the recurrence */
    seq_t a_prev3 = (seq_t)1;  /* a(1) */
    seq_t a_prev2 = (seq_t)2;  /* a(2) */
    seq_t a_prev1 = (seq_t)5;  /* a(3) */
    seq_t current;

    for (idx_t i = 4; i <= n; ++i) {
        /* current = a_prev1 + a_prev2 + a_prev3 + 2*i - 4 */
        current = a_prev1 + a_prev2 + a_prev3;
        current = current + ((seq_t)2 * (seq_t)i - (seq_t)4);

        /* Shift the window for next iteration */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = current;
    }

    return a_prev1;
}
