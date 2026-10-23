/*
 * A047482  Numbers that are congruent to {1, 2, 5, 7} mod 8.
 * https://oeis.org/A047482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4)+{1,2,5,7}[(n-1)%4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* numbers congruent to 1, 2, 5, or 7 modulo 8 */
    if (n == 0)          /* undefined for offset 0, but harmless */
        return 0;

    idx_t block = (n - 1) / 4;          /* 0‑based block index   */
    unsigned pos = (unsigned)((n - 1) % 4);  /* 0‑based position      */

    static const seq_t offs[4] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)7 };

    return ((seq_t)8 * (seq_t)block) + offs[pos];
}
