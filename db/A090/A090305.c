/*
 * A090305  a(n) = 16*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 16.
 * https://oeis.org/A090305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-1) + a(n-2), a(0) = 2, a(1) = 16
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 16;

    seq_t prev2 = 2;   /* a(0) */
    seq_t prev1 = 16;  /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (prev1 << 4) + prev2; /* 16*prev1 + prev2 */
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
