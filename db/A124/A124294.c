/*
 * A124294  Number of free generators of degree n of symmetric polynomials in
 *          6-noncommuting variables.
 * https://oeis.org/A124294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 15*a(n-1) - 81*a(n-2) + 192*a(n-3) - 189*a(n-4) + 53*a(n-5)
   with initial values:
   a(1)=1, a(2)=1, a(3)=2, a(4)=6, a(5)=22 */
seq_t a(idx_t n)
{
    /* Initial terms a(1) .. a(5) */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)6, (seq_t)22 };

    if (n <= 5)
        return init[n - 1];

    /* Use a sliding window for the last five values */
    seq_t a1 = init[0];   /* a(n-5) */
    seq_t a2 = init[1];   /* a(n-4) */
    seq_t a3 = init[2];   /* a(n-3) */
    seq_t a4 = init[3];   /* a(n-2) */
    seq_t a5 = init[4];   /* a(n-1) */
    seq_t an = 0;         /* current a(n) */

    for (idx_t k = 6; k <= n; ++k) {
        /* Compute a(k) using the recurrence */
        seq_t pos = (seq_t)15 * a5
                  + (seq_t)192 * a3
                  + (seq_t)53  * a1;
        seq_t neg = (seq_t)81  * a4
                  + (seq_t)189 * a2;
        an = pos - neg;   /* safe: pos >= neg for all k */

        /* Shift the window forward for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
