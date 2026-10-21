/*
 * A070093  Number of acute integer triangles with perimeter n.
 * https://oeis.org/A070093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered integer triangles (a,b,c) with
 *            a+b+c=n, a≤b≤c, satisfying the triangle inequality a+b>c and the
 *            acute condition a²+b²>c²
 * Range:     u8 n <= 32, u16 n <= 559, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t count = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t max_b = (n - a) / 2;  /* ensures b ≤ c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            /* Triangle inequality: a + b > c is automatic when b ≤ c,
               but check for safety. */
            if (a + b <= c) continue;
            seq_t a2 = (seq_t)a * (seq_t)a;
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t c2 = (seq_t)c * (seq_t)c;
            if (a2 + b2 > c2) ++count;
        }
    }
    return count;
}
