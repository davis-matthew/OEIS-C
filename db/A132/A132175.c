/*
 * A132175  Index of end of n-th generation of terms in A063882.
 * https://oeis.org/A132175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) + 3*(n-3)  for n >= 14
   Initial values (offset 1):
   a(1) = 4
   a(2) = 9
   a(3) = 20
   a(4) = 43
   a(5) = 91
   a(6) = 188
   a(7) = 384
   a(8) = 777
   a(9) = 1564
   a(10) = 3140
   a(11) = 6293
   a(12) = 12601
   a(13) = 25218
*/
seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* undefined for offset 1, but safe guard */
    }
    static const seq_t known[14] = {
        0,      /* placeholder for index 0 */
        4, 9, 20, 43, 91, 188, 384, 777,
        1564, 3140, 6293, 12601, 25218
    };
    if (n <= 13) {
        return known[n];
    }

    seq_t prev2 = known[12]; /* a(n-2) */
    seq_t prev1 = known[13]; /* a(n-1) */

    for (idx_t i = 14; i <= n; ++i) {
        seq_t term = prev1 + prev2 * (seq_t)2 + (seq_t)3 * (seq_t)(i - 3);
        prev2 = prev1;
        prev1 = term;
    }
    return prev1;
}
