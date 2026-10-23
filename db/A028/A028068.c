/*
 * A028068  Expansion of 1/((1-3x)(1-5x)(1-9x)(1-10x)).
 * https://oeis.org/A028068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=27, a(2)=472, a(3)=6810;
   a(n)=27*a(n-1)-257*a(n-2)+1005*a(n-3)-1350*a(n-4) for n≥4. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)472;
    if (n == 3) return (seq_t)6810;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)27;  /* a(1) */
    seq_t a2 = (seq_t)472; /* a(2) */
    seq_t a3 = (seq_t)6810;/* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term using the recurrence.
           To avoid underflow in unsigned types, we write the expression
           as the difference of two non‑negative sums. */
        seq_t term1 = (seq_t)27 * a3 + (seq_t)1005 * a1;
        seq_t term2 = (seq_t)257 * a2 + (seq_t)1350 * a0;
        seq_t next  = term1 - term2;

        a0 = a1; a1 = a2; a2 = a3; a3 = next;
    }
    return a3;
}
