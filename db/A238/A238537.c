/*
 * A238537  A fourth-order linear divisibility sequence related to the Pell
 *          numbers.
 * https://oeis.org/A238537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 28*a(n-1)+202*a(n-2)+28*a(n-3)-a(n-4)
   with a(1)=1, a(2)=42, a(3)=1379, a(4)=47124 */

seq_t a(idx_t n) {
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)42;
    if (n == 3) return (seq_t)1379;
    if (n == 4) return (seq_t)47124;

    /* Iteratively compute using the linear recurrence */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)42;
    seq_t a3 = (seq_t)1379;
    seq_t a4 = (seq_t)47124;
    seq_t an = a4;  /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)28 * a4 + (seq_t)202 * a3 + (seq_t)28 * a2 - a1;
        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
