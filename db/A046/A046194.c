/*
 * A046194  Heptagonal triangular numbers.
 * https://oeis.org/A046194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+103682*a(n-2)-103682*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=1, a(2)=55, a(3)=121771, a(4)=5720653,
 *            a(5)=12625478965
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, though n ≥ 1 by definition */

    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)55,
        (seq_t)121771,
        (seq_t)5720653,
        (seq_t)12625478965ULL
    };

    if (n <= 5) return init[n-1];

    seq_t buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = init[i];

    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = buf[4]                       /* a(k-1) */
                   + (seq_t)103682 * buf[3]       /* +103682*a(k-2) */
                   - (seq_t)103682 * buf[2]       /* -103682*a(k-3) */
                   - buf[1]                       /* -a(k-4) */
                   + buf[0];                      /* +a(k-5) */

        /* shift buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
