/*
 * A193385  Expansion of e.g.f. cosh( x^2/2 )/ (1-x).
 * https://oeis.org/A193385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0}^{⌊n/4⌋} 1/(4^k * (2k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1; for n>0,
   if n%4==0: a(n)=n*a(n-1)+n!/(4^{n/4}*(n/2)!)
   else:     a(n)=n*a(n-1)                            */

seq_t a(idx_t n)
{
    seq_t a_val = 1;          /* a(0) */
    if (n == 0) return a_val;

    seq_t fact = 1;           /* running factorial n!          */
    seq_t half_fact = 1;      /* factorial of (i/2) when i is even */
    seq_t pow4 = 1;           /* 4^{i/4} when i divisible by 4   */

    for (idx_t i = 1; i <= n; ++i) {
        fact = fact * (seq_t)i;          /* update factorial */

        if ((i & 1) == 0) {              /* even i: update half_factorial */
            idx_t j = i >> 1;            /* j = i/2                     */
            half_fact = half_fact * (seq_t)j;
        }

        if ((i & 3) == 0) {              /* i divisible by 4            */
            pow4 = pow4 * (seq_t)4;
            seq_t den = pow4 * half_fact;/* denominator 4^{i/4} * (i/2)! */
            seq_t term = fact / den;     /* exact integer division */
            a_val = a_val * (seq_t)i + term;
        } else {
            a_val = a_val * (seq_t)i;    /* a(n)=n*a(n-1) for other n */
        }
    }

    return a_val;
}
