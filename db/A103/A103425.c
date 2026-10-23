/*
 * A103425  a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3).
 * https://oeis.org/A103425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103425: a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3)  */
/* Initial values: a(0) = 1, a(1) = 3, a(2) = 5 */
/* (Equivalently, a(n) = 1 + (3^n - (-1)^n)/2) */

seq_t a(idx_t n) {
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* Iteratively compute subsequent terms */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)3;  /* a(n-2) */
    seq_t a2 = (seq_t)5;  /* a(n-1) */
    seq_t an;            /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 3 * a2 + a1 - 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
