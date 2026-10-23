/*
 * A147757  Palindromes formed from the reflected decimal expansion of the
 *          concatenation of 1, 0 and infinite digits 1.
 * https://oeis.org/A147757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) for n > 5 Initial values: a(1)=1,
 *            a(2)=11, a(3)=101, a(4)=1001, a(5)=10101
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)101;
    if (n == 4) return (seq_t)1001;
    if (n == 5) return (seq_t)10101;

    /* For n >= 6 we use the recurrence. */
    seq_t prev2 = (seq_t)1001;   /* a(4) */
    seq_t prev1 = (seq_t)10101;  /* a(5) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t curr = (seq_t)11 * prev1 - (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
