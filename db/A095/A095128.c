/*
 * A095128  a(n+3) = 3*a(n+2) + 2*a(n+1) - a(n).
 * https://oeis.org/A095128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3); a(1)=1, a(2)=4, a(3)=13
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is irrelevant */
        return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 13;

    seq_t a1 = 1;   /* a(n-3) */
    seq_t a2 = 4;   /* a(n-2) */
    seq_t a3 = 13;  /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 3 * a3 + 2 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
