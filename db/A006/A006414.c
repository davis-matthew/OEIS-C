/*
 * A006414  Number of nonseparable toroidal tree-rooted maps with n + 2 edges
 *          and n + 1 vertices.
 * https://oeis.org/A006414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^3*(n+3)/24
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 157, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 2;
    seq_t f[5];
    int i, k;
    seq_t r;
    f[0] = m - 1;
    f[1] = m;
    f[2] = m;
    f[3] = m;
    f[4] = m + 1;
    /* divide out 3 from a factor divisible by 3 */
    for (i = 0; i < 5; i++) {
        if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }
    /* divide out 2 three times from even factors */
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 5; i++) {
            if (f[i] % 2 == 0) { f[i] /= 2; break; }
        }
    }
    r = 1;
    for (i = 0; i < 5; i++) r *= f[i];
    return r;
}
