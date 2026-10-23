/*
 * A037560  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2. For n > 1, a(n) = 3 * a(n-1) + ( (n-1) % 3 == 2 ? 1 :
 *            2 ). This generates the base‑3 number whose digits form the
 *            periodic sequence 2, 2, 1, 2, 2, 1, … (most significant digit
 *            first)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset is 1, but guard anyway */
    seq_t val = 2;                            /* a(1) */
    for (idx_t i = 1; i < n; ++i) {           /* i = n‑1 of the recurrence */
        val = val * 3 + ((i % 3) == 2 ? 1 : 2);
    }
    return val;
}
