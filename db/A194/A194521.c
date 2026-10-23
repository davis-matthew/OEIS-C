/*
 * A194521  Second coordinate of (3,8)-Lagrange pair for n.
 * https://oeis.org/A194521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n - 4*⌊(7*n+5)/11⌋ + 4*⌊(7*n+6)/11⌋ - 3*⌊(7*n+7)/11⌋
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A194521) */
seq_t a(idx_t n)
{
    /* Handle the finite initial segment explicitly */
    static const seq_t init[13] = {
        0,   /* index 0 unused */
        -1,  /* a(1) */
        1,   /* a(2) */
        0,   /* a(3) */
        -1,  /* a(4) */
        1,   /* a(5) */
        0,   /* a(6) */
        3,   /* a(7) */
        1,   /* a(8) */
        0,   /* a(9) */
        -1,  /* a(10) */
        1,   /* a(11) */
        0    /* a(12) */
    };

    if (n <= 12U)
        return init[n];

    /* Compute the floor terms using unsigned arithmetic */
    idx_t f1i = (7 * n + 5) / 11;
    idx_t f2i = (7 * n + 6) / 11;
    idx_t f3i = (7 * n + 7) / 11;

    seq_t n_t  = (seq_t)n;
    seq_t f1   = (seq_t)f1i;
    seq_t f2   = (seq_t)f2i;
    seq_t f3   = (seq_t)f3i;

    return   (seq_t)2 * n_t
           - (seq_t)4 * f1
           + (seq_t)4 * f2
           - (seq_t)3 * f3;
}
