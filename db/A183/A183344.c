/*
 * A183344  Number of n X 2 binary arrays with each 1 adjacent to exactly one
 *          0 vertically and one 0 horizontally.
 * https://oeis.org/A183344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + a(n-3)
   Initial values: a(1)=1, a(2)=7, a(3)=9 */
seq_t a(idx_t n)
{
    /* Handle request for index 0 – not defined, return 0 */
    if (n == 0) return (seq_t)0;

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)9;

    /* Iteratively compute the recurrence, using only the last three terms */
    seq_t a1 = (seq_t)1;   /* a(i-3) */
    seq_t a2 = (seq_t)7;   /* a(i-2) */
    seq_t a3 = (seq_t)9;   /* a(i-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = a3 + a2 + a1; /* a(i) = a(i-1)+a(i-2)+a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
