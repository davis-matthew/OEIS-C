/*
 * A198482  Number of 2n X 2 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to an even number of horizontal
 *          and vertical neighbors.
 * https://oeis.org/A198482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 55*a(n-1) - 135*a(n-2) + 81*a(n-3) for n > 4
   Initial values:  a(1)=5,  a(2)=193,  a(3)=10072,  a(4)=528283 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour below is irrelevant */
    if (n == 1) return 5;
    if (n == 2) return 193;
    if (n == 3) return 10072;
    if (n == 4) return 528283;

    /* Use the recurrence for n >= 5 */
    seq_t prev1 = 528283; /* a(n-1)   for n=5 */
    seq_t prev2 = 10072;  /* a(n-2)   for n=5 */
    seq_t prev3 = 193;    /* a(n-3)   for n=5 */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 55*a(i-1) - 135*a(i-2) + 81*a(i-3) */
        seq_t next = (55 * prev1 + 81 * prev3) - 135 * prev2;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
