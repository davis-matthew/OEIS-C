/*
 * A019677  Expansion of 1/((1-4x)(1-8x)(1-12x)).
 * https://oeis.org/A019677
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

/* OEIS A019677 – Expansion of 1/((1-4x)(1-8x)(1-12x))
   Formula: a(n) = (4^n - 8·8^n + 9·12^n)/2,  n ≥ 0
   Recurrence (for all n ≥ 3):
       a(0) = 1
       a(1) = 24
       a(2) = 400
       a(n) = 24·a(n-1) – 176·a(n-2) + 384·a(n-3)
*/

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)400;

    /* iterate using recurrence */
    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 24;  /* a(1) */
    seq_t a2 = 400; /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = a2 * (seq_t)24;
        seq_t t2 = a1 * (seq_t)176;
        seq_t t3 = a0 * (seq_t)384;
        next = t1 - t2 + t3;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
