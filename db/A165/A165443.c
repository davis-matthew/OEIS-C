/*
 * A165443  a(n) = ( 16^(2*n+1) + 81^(2*n+1) )/97.
 * https://oeis.org/A165443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^(2*n+1) + 81^(2*n+1)) / 97
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5521,
   a(n) = 6817*a(n-1) - 1679616*a(n-2) for n>=2 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5521;

    seq_t prev2 = (seq_t)1;      /* a(n-2) */
    seq_t prev1 = (seq_t)5521;   /* a(n-1) */
    seq_t curr = prev1;          /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)6817 * prev1 - (seq_t)1679616 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
