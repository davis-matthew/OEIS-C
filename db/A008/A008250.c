/*
 * A008250  Coordination sequence T2 for Banalsite.
 * https://oeis.org/A008250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-5) - a(n-7) + a(n-8) (n ≥ 12)
 *            Initial values: a(0)=1, a(1)=4, a(2)=11, a(3)=24, a(4)=42,
 *            a(5)=65, a(6)=97, a(7)=135, a(8)=176, a(9)=221, a(10)=277,
 *            a(11)=337
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* known first twelve terms */
    static const seq_t base[12] = {
        (seq_t)1,  (seq_t)4,  (seq_t)11, (seq_t)24,
        (seq_t)42, (seq_t)65, (seq_t)97,  (seq_t)135,
        (seq_t)176,(seq_t)221,(seq_t)277, (seq_t)337
    };

    /* Return the value directly for the initial window. */
    if (n < 12)
        return base[n];

    /* Sliding window of the last eight values:
       prev[0] = a(i-8), ..., prev[7] = a(i-1)   (for i=12 initially)
    */
    seq_t prev[8];
    prev[0] = base[4];  /* a4 */
    prev[1] = base[5];  /* a5 */
    prev[2] = base[6];  /* a6 */
    prev[3] = base[7];  /* a7 */
    prev[4] = base[8];  /* a8 */
    prev[5] = base[9];  /* a9 */
    prev[6] = base[10]; /* a10 */
    prev[7] = base[11]; /* a11 */

    seq_t cur = 0;
    for (idx_t i = 12; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-3) - a(i-5) - a(i-7) + a(i-8) */
        cur = prev[7] + prev[5] - prev[3] - prev[1] + prev[0];

        /* slide the window one step forward */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = prev[7];
        prev[7] = cur;
    }
    return cur;
}
