/*
 * A202196  Number of (n+2) X 4 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n+4)*(n+3)*(n+2)^2/3
 * Range:     u8 none, u16 n <= 8, u32 n <= 182, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The expression is always integral for n ≥ 1 because (n+2)(n+3)(n+4) is divisible by 3. */

seq_t a(idx_t n)
{
    /* For n = 0 the offset is undefined; return 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Convert the index to the sequence type once. */
    seq_t nn = (seq_t)n;

    /* Compute the terms (n+2), (n+3), (n+4). */
    seq_t t1 = nn + (seq_t)2;   /* n + 2 */
    seq_t t2 = nn + (seq_t)3;   /* n + 3 */
    seq_t t3 = nn + (seq_t)4;   /* n + 4 */

    /* Evaluate 4*(n+4)*(n+3)*(n+2)^2 / 3 using only integer ops. */
    seq_t result = t1 * t1;     /* (n+2)^2 */
    result *= t2;              /* * (n+3) */
    result *= t3;              /* * (n+4) */
    result *= (seq_t)4;        /* * 4 */
    result /= (seq_t)3;        /* / 3 */

    return result;
}
