/*
 * A210842  Number of states in the n-th shell of the nuclear shell model.
 * https://oeis.org/A210842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = number of states in the n-th shell of the nuclear shell
 *            model
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1,2,3: a(n) = n*(n+1) */
/* For n = 4: a(4) = 8 */
/* For n >= 5: a(n) = n*(n+1) - 2*(n-1) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, safe guard */
    /* cast to seq_t before multiplication to avoid overflow of idx_t */
    seq_t t = (seq_t)n * (seq_t)(n + 1);

    if (n <= 3)
        return t;                          /* 2,6,12 */
    if (n == 4)
        return (seq_t)8;                   /* 8 */
    /* n >= 5 */
    return t - (seq_t)(2 * (n - 1)); /* n*(n+1) - 2*(n-1) */
}
