/*
 * A025007  Expansion of 1/((1-x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A025007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A025007
   Recurrence: a(0)=1, a(1)=31, a(2)=635, a(3)=10835
   a(n) = 31*a(n-1) - 326*a(n-2) + 1256*a(n-3) - 960*a(n-4)  (n ≥ 4) */
seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)635;
    if (n == 3) return (seq_t)10835;

    /* Sliding window for recurrence */
    seq_t a0 = (seq_t)1;        /* a(n-4) */
    seq_t a1 = (seq_t)31;       /* a(n-3) */
    seq_t a2 = (seq_t)635;      /* a(n-2) */
    seq_t a3 = (seq_t)10835;    /* a(n-1) */
    seq_t curr = 0;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        curr = (seq_t)31 * a3 - (seq_t)326 * a2 + (seq_t)1256 * a1 - (seq_t)960 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
