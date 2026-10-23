/*
 * A106837  Numbers m such that both m and m+1 have odd part of the form
 *          4*k+3.
 * https://oeis.org/A106837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that odd_part(m) ≡ 3 (mod 4) and *
 *            odd_part(m+1) ≡ 3 (mod 4)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    seq_t m = 1;                   /* candidate value */
    idx_t count = 0;               /* how many found so far */

    for (;;) {
        /* odd part of m */
        seq_t x = m;
        while ((x & 1) == 0) x >>= 1;
        if ((x & 3) != 3) {        /* fails first test, skip */
            ++m;
            continue;
        }

        /* odd part of m+1 */
        seq_t y = m + 1;
        while ((y & 1) == 0) y >>= 1;
        if ((y & 3) == 3) {
            ++count;
            if (count == n) return m;   /* found the n-th term */
        }
        ++m;                          /* try next candidate */
    }
}
