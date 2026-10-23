/*
 * A107476  Expansion of
 *          -x*(-1+5*x+8*x^2-11*x^3+3*x^4)/(1-6*x-4*x^2+24*x^3-6*x^4-4*x^5+x^6).
 * https://oeis.org/A107476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 6*a(n-1)+4*a(n-2)-24*a(n-3)+6*a(n-4)+4*a(n-5)-a(n-6) with
 *            initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the linear recurrence derived from the rational generating function
   of OEIS sequence A107476. It works for any idx_t n >= 0. */
seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    switch (n) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 1;
        case 3: return 2;
        case 4: return 3;
        case 5: return 5;
        default: break;
    }

    /* Small buffer to hold the last six values of the sequence */
    seq_t v[6] = {0, 1, 1, 2, 3, 5};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 6*v[5] + 4*v[4] - 24*v[3] + 6*v[2] + 4*v[1] - v[0];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];
}
