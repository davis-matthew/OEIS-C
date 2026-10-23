/*
 * A019628  Expansion of 1/((1-4*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A019628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=23, a(2)=369; for n>2  
 * a(n) = 23*a(n-1) - 160*a(n-2) + 336*a(n-3)
 */
seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)369;

    /* Iteratively compute using the recurrence */
    seq_t a0 = (seq_t)1;    /* a(n-3) */
    seq_t a1 = (seq_t)23;   /* a(n-2) */
    seq_t a2 = (seq_t)369;  /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t ai = 23 * a2 - 160 * a1 + 336 * a0; /* a(n) */
        a0 = a1;  /* shift: a(n-3) becomes old a(n-2) */
        a1 = a2;  /* shift: a(n-2) becomes old a(n-1) */
        a2 = ai;  /* shift: a(n-1) becomes new a(n) */
    }
    return a2; /* a(n) after loop */
}
