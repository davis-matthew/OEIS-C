/*
 * A167567  The fourth left hand column of triangle A167565.
 * https://oeis.org/A167567
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8); Initial values (for n = 4..11):
 *            a(4)=0, a(5)=14, a(6)=124, a(7)=601, a(8)=2120, a(9)=6096,
 *            a(10)=15168, a(11)=33858. The recurrence holds for all integer n
 *            ≥ 12
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below returns a(n) for any n ≥ 4.  It is completely
   self‑contained, requires no dynamic allocation and works for all
   allowed seq_t types (built‑in integers or arbitrary‑precision
   big‑int classes). */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* undefined below offset */

    /* Direct return for the first eight known terms. */
    if (n <= 11) {
        static const seq_t init[8] = {
            (seq_t)0, (seq_t)14, (seq_t)124, (seq_t)601,
            (seq_t)2120, (seq_t)6096, (seq_t)15168, (seq_t)33858
        };
        return init[n - 4];
    }

    /* Compute the terms iteratively using a sliding window of the last
       eight values.  The window is stored in buf[0..7] such that:
       buf[0] = a(i-7), buf[1] = a(i-6), … , buf[7] = a(i). */
    seq_t buf[8] = {
        (seq_t)0,    (seq_t)14,    (seq_t)124,   (seq_t)601,
        (seq_t)2120, (seq_t)6096,  (seq_t)15168, (seq_t)33858
    };

    for (idx_t i = 12; i <= n; ++i) {
        seq_t next =
            (seq_t)8   * buf[7] - (seq_t)28  * buf[6]
          + (seq_t)56  * buf[5] - (seq_t)70  * buf[4]
          + (seq_t)56  * buf[3] - (seq_t)28  * buf[2]
          + (seq_t)8   * buf[1] -           buf[0];

        /* Shift the sliding window one position left. */
        for (idx_t k = 0; k < 7; ++k) buf[k] = buf[k + 1];
        buf[7] = next;
    }
    return buf[7];
}
