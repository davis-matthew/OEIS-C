/*
 * A047528  Numbers that are congruent to {0, 3, 7} mod 8.
 * https://oeis.org/A047528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers congruent to {0,3,7} modulo 8. Explicit case
 *            formulas: a(3k) = 8k-1 a(3k-1) = 8k-5 a(3k-2) = 8k-8 for k ≥ 1.
 *            Index offset is 1 (a(1)=0)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t rem = n % (idx_t)3;
    idx_t k;
    seq_t val;

    if (rem == (idx_t)1) {          /* n = 3k-2  →  a = 8k-8 */
        k = (n + (idx_t)2) / (idx_t)3;
        val = (seq_t)8 * (seq_t)k - (seq_t)8;
    } else if (rem == (idx_t)2) {   /* n = 3k-1  →  a = 8k-5 */
        k = (n + (idx_t)1) / (idx_t)3;
        val = (seq_t)8 * (seq_t)k - (seq_t)5;
    } else {                        /* rem == 0 : n = 3k   →  a = 8k-1 */
        k = n / (idx_t)3;
        val = (seq_t)8 * (seq_t)k - (seq_t)1;
    }
    return val;
}
