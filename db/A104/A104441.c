/*
 * A104441  Number of ways to split 1, 2, 3, ..., 8n into 8 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of ways to split 1,2,…,8n into 8 arithmetic progressions
 *            each of length n. The sequence stabilizes at 320 for all n ≥ 9
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[] = {
        (seq_t)1,
        (seq_t)2027025,
        (seq_t)6643,
        (seq_t)488,
        (seq_t)349,
        (seq_t)329,
        (seq_t)323,
        (seq_t)321
    };
    if (n == 0) return (seq_t)0;       /* Undefined for offset 0, but harmless */
    if (n <= 8) return tbl[n - 1];
    return (seq_t)320;
}
