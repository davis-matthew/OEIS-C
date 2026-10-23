/*
 * A192687  Male-female differences: a(n) = A005378(n) - A005379(n).
 * https://oeis.org/A192687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A192687: Male-female differences.
 * Formula: a(n) = 1 if n is a partial sum of Fibonacci numbers (i.e.,
 *                n = F1 + F2 + ... + Fk for some k ≥ 0, where F1=F2=1),
 *             otherwise a(n) = 0.
 *
 * Positions of 1's are thus 0, 1, 2, 4, 7, 12, 20, 33, 53, ...
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* the empty sum gives a(0)=1 */
        return (seq_t)1;

    idx_t sum   = 0;           /* running partial sum of Fibonacci numbers */
    idx_t f_prev = 1;          /* F1 */
    idx_t f_curr = 1;          /* F2 */

    /* Generate partial sums F1, F1+F2, F1+F2+F3, ... */
    while (1) {
        sum += f_prev;         /* add current Fibonacci number */

        if (sum == n)
            return (seq_t)1;   /* found n as a Fibonacci partial sum */

        if (sum > n)
            break;             /* n cannot be a partial sum any higher */

        /* Advance to the next Fibonacci number */
        idx_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return (seq_t)0;           /* n is not a partial sum of Fibonacci numbers */
}
