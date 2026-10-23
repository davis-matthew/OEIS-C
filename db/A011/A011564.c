/*
 * A011564  Stirling numbers of second kind S2(15,n).
 * https://oeis.org/A011564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stirling number of the second kind S(15,n) */
seq_t a(idx_t n)
{
    static seq_t table[16];          /* table[ k ] = S(15,k)   for 0 ≤ k ≤ 15 */
    static int built = 0;            /* flag indicating that the table is ready */

    if (!built) {
        seq_t prev[16] = {0};
        seq_t cur[16]  = {0};

        prev[0] = 1;                 /* S(0,0) = 1 */
        for (idx_t i = 1; i <= 15; ++i) {
            cur[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                cur[k] = (seq_t)k * prev[k] + prev[k - 1];
            }
            for (idx_t k = 0; k <= i; ++k) {
                prev[k] = cur[k];
            }
        }
        for (idx_t k = 0; k <= 15; ++k) {
            table[k] = prev[k];
        }
        built = 1;
    }

    if (n > 15)
        return (seq_t)0;
    if (n < 1)
        return (seq_t)0;      /* offset is 1; behaviour below offset is unspecified */
    return table[n];
}
