/*
 * A206089  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-3) for n>6, with a(1)=972, a(2)=2016, a(3)=4560,
 *            a(4)=13344, a(5)=33792, a(6)=86496. This implements the OEIS
 *            sequence A206089. The function is defined for all n≥1 in the
 *            usual OEIS offset
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed initial terms a(1)..a(6). */
    static const seq_t init[6] = {972, 2016, 4560, 13344, 33792, 86496};

    if (n <= 6)
        return init[n - 1];

    /* For n > 6 use the recurrence a(n) = 18*a(n-3).  Reduce n until
       it lies in the range 4–6; let base_idx be that value (4,5,or6). */
    idx_t r = (n - 4) % 3;          /* 0,1,2 – cycle over 4,5,6 */
    idx_t base_idx = 4 + r;         /* 4,5,6 */
    idx_t k = (n - base_idx) / 3;   /* number of multiplications by 18 */

    seq_t result = init[base_idx - 1]; /* a(4), a(5), or a(6) */
    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)18;

    return result;
}
