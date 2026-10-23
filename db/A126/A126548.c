/*
 * A126548  Number of base 27 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   Let f[d] be the number of n‑digit base‑27 strings ending with digit d.
   Base:   f[d] = 1  for all d (n = 1)
   Transition:   f_next[d] = Σ_{e:|e-d|≤5} f[e]
   Result: a(n) = Σ_{d=0}^{26} f[d]   (a(0) = 1 by definition)
   This counts all strings of length n (including leading zeros) in which
   each pair of adjacent digits differs by at most 5. */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                     /* a(0) = 1 */

    /* dp_prev[d] – number of strings of current length ending with digit d */
    seq_t dp_prev[27];
    seq_t dp_cur[27];

    /* Base case: length 1 */
    for (int d = 0; d < 27; ++d) dp_prev[d] = 1;

    if (n == 1) {                               /* a(1) = 27 */
        seq_t total = 0;
        for (int d = 0; d < 27; ++d) total += dp_prev[d];
        return total;
    }

    /* Build lengths 2 … n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 27; ++d) {
            seq_t sum = 0;
            int lo = d - 5;
            if (lo < 0) lo = 0;
            int hi = d + 5;
            if (hi > 26) hi = 26;
            for (int e = lo; e <= hi; ++e)
                sum += dp_prev[e];
            dp_cur[d] = sum;
        }
        /* Prepare for next iteration */
        for (int d = 0; d < 27; ++d)
            dp_prev[d] = dp_cur[d];
    }

    /* Sum over final digits to obtain a(n) */
    seq_t total = 0;
    for (int d = 0; d < 27; ++d)
        total += dp_prev[d];

    return total;
}
