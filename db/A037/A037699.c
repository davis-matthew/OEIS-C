/*
 * A037699  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Base-8 digits are the first n terms of the periodic sequence 1,2,3,0.
   Recurrence: a(0) = 0, a(k) = a(k-1)*8 + d_k  for k ≥ 1,
   where d_k = {1,2,3,0} repeated.
   The function returns a(n) for n ≥ 1. 
*/
seq_t a(idx_t n) {
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = (i - 1) % 4;          /* 0,1,2,3 cycle */
        seq_t digit = (r == 3) ? 0 : (seq_t)(r + 1);
        val = val * 8 + digit;
    }
    return val;
}
