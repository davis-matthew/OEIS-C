/*
 * A160545  Numbers coprime to 21.
 * https://oeis.org/A160545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160545: Numbers coprime to 21
   Base pattern (length 12):
   1,2,4,5,8,10,11,13,16,17,19,20
   Periodicity: a(n+12) = a(n) + 21
*/

seq_t a(idx_t n) {
    static const seq_t tbl[12] = {1, 2, 4, 5, 8, 10, 11, 13, 16, 17, 19, 20};

    if (n == 0) return 0;  /* undefined but safe fallback */

    /* index offset within one period */
    idx_t r = (n - 1) % 12;
    /* number of complete 21‑integer blocks before n */
    idx_t blk = (n - 1) / 12;

    /* compute a(n) = base[r] + 21 * blk */
    seq_t result = tbl[r] + (seq_t)21 * (seq_t)blk;
    return result;
}
