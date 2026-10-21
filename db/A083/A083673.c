/*
 * A083673  First differences have period 6: 2,-1,3,4,-3,7, 2,-1,3,4,-3,7,
 *          2,-1,...
 * https://oeis.org/A083673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-6)+12 for n>6; base terms a(1)=2, a(2)=4, a(3)=3, a(4)=6, a(5)=10, a(6)=7 */
seq_t a(idx_t n) {
    static seq_t base[6] = { (seq_t)2, (seq_t)4, (seq_t)3, (seq_t)6, (seq_t)10, (seq_t)7 };
    idx_t off = n - 1;  /* offset from first term (zero‑based) */
    seq_t cycle = (seq_t)12 * (seq_t)(off / 6);
    return base[off % 6] + cycle;
}
