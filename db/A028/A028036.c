/*
 * A028036  Expansion of 1/((1-3x)(1-4x)(1-6x)(1-11x)).
 * https://oeis.org/A028036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*11^(n+3)-28*6^(n+3)+60*4^(n+3)-35*3^(n+3))/840
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the linear recurrence
   a(n) = 24*a(n-1) -197*a(n-2) +666*a(n-3) -792*a(n-4)
   with a(0)=1, a(1)=24, a(2)=379, a(3)=5034. */

seq_t a(idx_t n)
{
    /* exact values for n = 0 … 3 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)379;
    if (n == 3) return (seq_t)5034;

    /* iterate from 4 up to n keeping only the last four values */
    seq_t a0 = (seq_t)1;    /* a(n-4) */
    seq_t a1 = (seq_t)24;   /* a(n-3) */
    seq_t a2 = (seq_t)379;  /* a(n-2) */
    seq_t a3 = (seq_t)5034; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* compute terms using the recurrence */
        seq_t t1 = 24 * a3;     /* 24*a(n-1)   */
        seq_t t2 = 197 * a2;    /* 197*a(n-2)  */
        seq_t t3 = 666 * a1;    /* 666*a(n-3)  */
        seq_t t4 = 792 * a0;    /* 792*a(n-4)  */

        /* avoid unsigned underflow: compute  (t1+t3) - (t2+t4) */
        seq_t positive = t1 + t3;
        seq_t negative = t2 + t4;
        seq_t next = positive - negative;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
