/*
 * A075848  Numbers k such that 2*k^2 + 9 is a square.
 * https://oeis.org/A075848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) with a(0) = 0 and a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)6;   /* a(n-1) */
    seq_t cur, mult;

    for (idx_t i = 2; i <= n; ++i) {
        /* Use a wider intermediate if seq_t is a built‑in type
           (e.g. uint64_t) to avoid overflow during multiplication. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 t = (unsigned __int128)prev1 * 6;
            mult = (seq_t)t;
        } else {
            mult = prev1 * (seq_t)6;
        }
        cur = mult - prev2;  /* a(i) = 6*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
