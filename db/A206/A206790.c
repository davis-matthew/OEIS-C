/*
 * A206790  Number of nX1 0..3 arrays avoiding the pattern z z+1 z in any row,
 *          column, diagonal or antidiagonal.
 * https://oeis.org/A206790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + a(n-3) with initial values a(1)=4,
 *            a(2)=16, a(3)=61
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1 – guard against underflow */

    /* Handle the first three terms directly.  */
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 61;

    /* Iteratively compute a(n) using the recurrence. */
    seq_t t1 = 4;   /* a(1) */
    seq_t t2 = 16;  /* a(2) */
    seq_t t3 = 61;  /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 4 * t3 - t2 + t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
