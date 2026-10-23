/*
 * A199910  Number of -n..n arrays x(0..2) of 3 elements with zero sum, and
 *          adjacent elements not equal modulo three (with -1 modulo 3 = 2).
 * https://oeis.org/A199910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5); a(1)=6,
 *            a(2)=12, a(3)=24, a(4)=42, a(5)=60
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[5] = {(seq_t)6, (seq_t)12, (seq_t)24, (seq_t)42, (seq_t)60};
    if (n <= 5) return init[n-1];
    seq_t prev[5];
    for (idx_t i = 0; i < 5; ++i) prev[i] = init[i];
    seq_t next = 0;
    for (idx_t i = 6; i <= n; ++i) {
        next = ((seq_t)2) * prev[4] - prev[3] + prev[2] - ((seq_t)2) * prev[1] + prev[0];
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }
    return next;
}
