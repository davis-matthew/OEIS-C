/*
 * A228245  The integers occurring in the song "Ten green bottles".
 * https://oeis.org/A228245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (n % 3 == 0) ? 1 : 10 - ((n - 1) / 3) (defined for n ≥ 1)
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out-of-range: not used in tests */
    idx_t block = (n - 1) / 3;       /* 0‑based block number */
    if (block >= 10) return 0;       /* the song contains only 10 … 1 */
    seq_t k = (seq_t)(10 - block);   /* current bottle count */
    return (n % 3 == 0) ? 1 : k;     /* pattern 10,10,1,9,9,1,… */
}
