/*
 * A124293  Number of free generators of degree n of symmetric polynomials in
 *          5-noncommuting variables.
 * https://oeis.org/A124293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(3)=2, a(4)=6;
   a(n) = 10*a(n-1) - 32*a(n-2) + 37*a(n-3) - 11*a(n-4)  for n>=5 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1, not used */

    /* Handle the first four values directly */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)6;
        default: break;
    }

    /* Use only the last four values to keep memory constant */
    seq_t a1 = (seq_t)1; /* a(n-4)  */
    seq_t a2 = (seq_t)1; /* a(n-3)  */
    seq_t a3 = (seq_t)2; /* a(n-2)  */
    seq_t a4 = (seq_t)6; /* a(n-1)  */
    seq_t next = 0;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t temp1 = (seq_t)10 * a4 + (seq_t)37 * a2;
        seq_t temp2 = (seq_t)32 * a3 + (seq_t)11 * a1;
        next = temp1 - temp2;

        /* shift last four values for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
