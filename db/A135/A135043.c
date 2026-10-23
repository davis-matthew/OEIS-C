/*
 * A135043  Duplicate of A054966.
 * https://oeis.org/A135043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the nth non‑negative integer that is 0, 1 or 8 modulo 9.
 *            (a(1)=0, a(2)=1, a(3)=8, a(4)=9, a(5)=10, …)
 * Range:     i8 n <= 44, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    /* For n ≥ 3, let m = n–3.  Then m/3 gives the number of full
       blocks of size 3 ending before the current term, and m%3
       indicates the offset within the current block (0->8, 1->9,
       2->10).  Each block contributes numbers 9k+8, 9k+9, 9k+10. */
    idx_t m = n - 3;
    idx_t q = m / 3;   /* block index */
    idx_t r = m % 3;   /* 0 → 8, 1 → 9, 2 → 10 */
    return (seq_t)q * 9 + 8 + (seq_t)r;
}
