/*
 * A151617  Row sums of A153521.
 * https://oeis.org/A151617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2) (n>4) Initial values: a(1)=2,
 *            a(2)=22, a(3)=242, a(4)=2662
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* Return directly for the first four terms. */
    switch ((uint64_t)n) {
        case 1: return (seq_t)2;
        case 2: return (seq_t)22;
        case 3: return (seq_t)242;
        case 4: return (seq_t)2662;
        default: break;
    }

    /* For n>4, compute iteratively using the recurrence. */
    seq_t prev2 = (seq_t)242;   /* a(3) */
    seq_t prev1 = (seq_t)2662;  /* a(4) */
    seq_t cur;

    /* Iterate from 5 up to n inclusive. */
    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)2 * prev1 + (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
