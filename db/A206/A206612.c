/*
 * A206612  Fibonacci sequence beginning 13, 6.
 * https://oeis.org/A206612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) for n>2; a(1) = 13, a(2) = 6
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two initial values explicitly */
    if (n == 1) return 13;
    if (n == 2) return 6;

    seq_t prev = 13; /* a(1) */
    seq_t curr = 6;  /* a(2) */

    /* iterate up to the required index */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}
