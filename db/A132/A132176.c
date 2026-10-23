/*
 * A132176  Value of A063882 at start of n-th generation of terms.
 * https://oeis.org/A132176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=12, a(5)=24 For n ≥ 6: a(n) =
 *            2*a(n‑1) + 1 if n mod 5 is 1 or 3, = 2*a(n‑1) otherwise
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1ULL) return 1;
    if (n == 2ULL) return 2;
    if (n == 3ULL) return 6;
    if (n == 4ULL) return 12;
    if (n == 5ULL) return 24;

    seq_t val = 24;
    /* iterate from 6 to n, updating val */
    for (idx_t i = 6; i <= n; ++i) {
        /* multiply by 2 */
        val = val << 1;
        /* add 1 when i ≡ 1 or 3 (mod 5) */
        if ((i % 5) == 1 || (i % 5) == 3) {
            val += 1;
        }
    }
    return val;
}
