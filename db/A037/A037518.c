/*
 * A037518  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1)+digit where digit cycles 2,0,1; a(1)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; behavior below offset is irrelevant */
    seq_t val = 2;                  /* a(1) */
    if (n == 1) return val;
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t m = (i - 1) % 3;   /* position of the new digit (0‑based) */
        seq_t d;
        if (m == 0) d = 2;
        else if (m == 1) d = 0;
        else d = 1;
        val = val * 9 + d;
    }
    return val;
}
