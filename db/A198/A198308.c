/*
 * A198308  Moore lower bound on the order of an (8,g)-cage.
 * https://oeis.org/A198308
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(2*i) = (7^i - 1)/3 * a(2*i+1) = (4*7^i - 1)/3 * with a(3)=9,
 *            a(4)=16, a(5)=65, a(6)=114, ... * The implementation uses the
 *            stable recurrence * b_{k+1} = 7*b_k + 2 * for each parity group
 *            separately, avoiding intermediate * overflow of 7^k
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* undefined for n<3 */

    if ((n & 1) == 0) { /* n = 2*i  (even) */
        idx_t i = n / 2;                /* i ≥ 2 for valid n */
        seq_t val = (seq_t)16;          /* a(4) = 16  (i = 2) */
        if (i < 2) return val;          /* safety for n = 4 */
        for (idx_t k = 2; k < i; ++k) {
            val = val * (seq_t)7 + (seq_t)2;
        }
        return val;
    } else { /* n = 2*i+1 (odd) */
        idx_t i = (n - 1) / 2; /* i ≥ 1 for n ≥ 3 */
        seq_t val = (seq_t)9;  /* a(3) = 9  (i = 1) */
        for (idx_t k = 1; k < i; ++k) {
            val = val * (seq_t)7 + (seq_t)2;
        }
        return val;
    }
}
