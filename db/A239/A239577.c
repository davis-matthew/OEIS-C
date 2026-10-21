/*
 * A239577  Expansion of 1/((x-1)*(3*x-1)*(3*x^2+1)).
 * https://oeis.org/A239577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A239577 (Expansion of 1/((x-1)*(3*x-1)*(3*x^2+1)))
 * Formula:  a(n) = 4*a(n-1) - 6*a(n-2) + 12*a(n-3) - 9*a(n-4),  n>3
 * Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=28
 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)28;

    /* Use the recurrence iteratively; no dynamic allocation needed */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)4;   /* a(n-3)   */
    seq_t a2 = (seq_t)10;  /* a(n-2)   */
    seq_t a3 = (seq_t)28;  /* a(n-1)   */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)4 * a3 - (seq_t)6 * a2 + (seq_t)12 * a1 - (seq_t)9 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;   /* a(n) after loop */
}
