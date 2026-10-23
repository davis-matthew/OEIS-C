/*
 * A021279  Expansion of 1/((1-x)(1-2x)(1-8x)(1-12x)).
 * https://oeis.org/A021279
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

/* Recurrence: a(0)=1, a(1)=23, a(2)=371, a(3)=5227,
   a(n)=23*a(n-1)-158*a(n-2)+328*a(n-3)-192*a(n-4)  (n≥4) */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)371;
    if (n == 3) return (seq_t)5227;

    /* previous four values: a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)23;
    seq_t a2 = (seq_t)371;
    seq_t a3 = (seq_t)5227;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)23 * a3
                    - (seq_t)158 * a2
                    + (seq_t)328 * a1
                    - (seq_t)192 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
