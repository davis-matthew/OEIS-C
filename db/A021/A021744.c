/*
 * A021744  Expansion of 1/((1-x)(1-4x)(1-5x)(1-6x)).
 * https://oeis.org/A021744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(2)=167, a(3)=1442
   a(n) = 16*a(n-1) + 194*a(n-3) - 89*a(n-2) - 120*a(n-4)  for n>=4 */

seq_t a(idx_t n)
{
    /* base values */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)16;
    seq_t a2 = (seq_t)167;
    seq_t a3 = (seq_t)1442;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute next term using only addition/subtraction of positives */
        seq_t t1 = (seq_t)16 * a3 + (seq_t)194 * a1;      /* 16*a(n-1)+194*a(n-3) */
        seq_t s1 = (seq_t)89 * a2 + (seq_t)120 * a0;      /* 89*a(n-2)+120*a(n-4) */
        seq_t next = t1 - s1;                             /* result is non‑negative */

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
