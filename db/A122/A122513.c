/*
 * A122513  Numbers n such that 1+2n+3n^2 is a triangular number.
 * https://oeis.org/A122513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 98*a(n-2) - 98*a(n-3) - a(n-4) + a(n-5)
   Initial values: a(1)=0, a(2)=1, a(3)=46, a(4)=135, a(5)=4540
   Offset: 1
*/

seq_t a(idx_t n)
{
    /* immediate return for the first five terms */
    switch (n) {
    case 1: return (seq_t)0;
    case 2: return (seq_t)1;
    case 3: return (seq_t)46;
    case 4: return (seq_t)135;
    case 5: return (seq_t)4540;
    default:
        /* variables holding the last five computed values
           a1 = a(n-4), a2 = a(n-3), a3 = a(n-2),
           a4 = a(n-1), a5 = a(n)              */
        seq_t a1 = (seq_t)0;     /* a(1) */
        seq_t a2 = (seq_t)1;     /* a(2) */
        seq_t a3 = (seq_t)46;    /* a(3) */
        seq_t a4 = (seq_t)135;   /* a(4) */
        seq_t a5 = (seq_t)4540;  /* a(5) */

        /* compute iteratively up to n */
        for (idx_t i = 6; i <= n; ++i) {
            seq_t new_val = a5
                + (seq_t)98 * a4
                - (seq_t)98 * a3
                - a2
                + a1;

            /* shift the window */
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = new_val;
        }
        return a5;  /* a(n) */
    }
}
