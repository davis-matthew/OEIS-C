/*
 * A021994  G.f.: 1/((1-x)(1-4x)(1-10x)(1-12x)).
 * https://oeis.org/A021994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A021994
 * Recurrence: a(0)=1, a(1)=27, a(2)=495, a(3)=7735,
 * a(n)=27*a(n-1)-234*a(n-2)+688*a(n-3)-480*a(n-4)  (n≥4)
 */

seq_t a(idx_t n)
{
    /* Small n: return the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)495;
    if (n == 3) return (seq_t)7735;

    /* Keep the last four values, indices: n-4, n-3, n-2, n-1 */
    seq_t a0 = (seq_t)1;     /* a(n-4) */
    seq_t a1 = (seq_t)27;    /* a(n-3) */
    seq_t a2 = (seq_t)495;   /* a(n-2) */
    seq_t a3 = (seq_t)7735;  /* a(n-1) */
    seq_t newv = 0;

    for (idx_t i = 4; i <= n; ++i) {
        newv = (seq_t)27 * a3
             - (seq_t)234 * a2
             + (seq_t)688 * a1
             - (seq_t)480 * a0;
        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = newv;
    }
    return a3;
}
