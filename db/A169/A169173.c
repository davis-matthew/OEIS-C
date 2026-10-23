/*
 * A169173  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 16 * 15^(n-1) for 1 ≤ n ≤ 26; for n ≥ 27 the
 *            sequence satisfies the linear recurrence a(n) = 14 *
 *            (a(n-1)+a(n-2)+...+a(n-26)) – 104 * a(n-27)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases first. */
    if (n == 0) return (seq_t)1;
    if (n <= 26) {
        seq_t val = (seq_t)16;          /* a(1) = 16 */
        for (idx_t i = 2; i <= n; ++i)
            val = val * (seq_t)15;      /* multiply by 15 for each increment */
        return val;
    }

    /* Allocate array to hold the values up to n.  n ≤ 10 000 for the tests, */
    /* so this memory consumption is trivial. */
    seq_t *a_val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_val) return (seq_t)0;   /* out of memory – unlikely in tests */

    /* Initialize the first 27 terms. */
    a_val[0] = (seq_t)1;
    seq_t t = (seq_t)16;            /* start with a(1) */
    for (idx_t i = 1; i <= 26; ++i) {
        a_val[i] = t;
        if (i < 26)                 /* prepare next term if needed */
            t = t * (seq_t)15;
    }

    /* Compute terms from 27 up to n using the recurrence. */
    for (idx_t i = 27; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 26; ++j)
            sum += a_val[i - j];
        a_val[i] = sum * (seq_t)14 - a_val[i - 27] * (seq_t)104;
    }

    seq_t result = a_val[n];
    free(a_val);
    return result;
}
