/*
 * A219160  Recurrence equation a(n+1) = a(n)^3 - 3*a(n) with a(0) = 4.
 * https://oeis.org/A219160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4, a(n+1) = a(n)^3 – 3*a(n) */
/* The values grow as a(n) = (2+√3)^(3^n)+(2-√3)^(3^n). */
seq_t a(idx_t n) {
    seq_t val = 4;
    if (n == 0) return val;
    for (idx_t i = 1; i <= n; ++i) {
        /* cubic term */
        seq_t cube = val;
        cube *= val;     /* val² */
        cube *= val;     /* val³ */
        /* recurrence step: val = val³ – 3·val */
        val = cube - 3*val;
    }
    return val;
}
