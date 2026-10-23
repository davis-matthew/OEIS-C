/*
 * A141499  a(0)=0; a(1)=1; a(n) = triangular number at index 5*2^(n-2)-1.
 * https://oeis.org/A141499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A141499
 * Formula: a(0) = 0; a(1) = 1; a(2) = 10; a(3) = 45;
 * Recurrence: a(n) = 6*a(n-1) - 8*a(n-2)   for n > 3
 */
seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 10;
    if (n == 3) return 45;

    /* Use the recurrence to iterate up to n */
    seq_t prev2 = 10;           /* a(n-2) for n=4 */
    seq_t prev1 = 45;           /* a(n-1) for n=4 */
    seq_t cur   = 0;

    /* Multipliers as seq_t to avoid implicit type issues */
    seq_t six  = 6;
    seq_t eight= 8;

    for (idx_t i = 4; i <= n; ++i) {
        cur = six * prev1 - eight * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
