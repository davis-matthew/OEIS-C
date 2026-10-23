/*
 * A188223  G.f.: (1+x^2+x^3)/(1-x-x^2-x^4-x^5).
 * https://oeis.org/A188223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=5, a(4)=9 a(n) = a(n-1) + a(n-2) +
 *            a(n-4) + a(n-5) for n ≥ 5
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)9;

    /* Maintain a rolling window of the last five values:
       w[0] = a(i-5), w[1] = a(i-4), w[2] = a(i-3),
       w[3] = a(i-2), w[4] = a(i-1)                     */
    seq_t w[5] = { (seq_t)1, /* a0 */
                   (seq_t)1, /* a1 */
                   (seq_t)3, /* a2 */
                   (seq_t)5, /* a3 */
                   (seq_t)9  /* a4 */ };

    /* Compute terms from a(5) up to a(n) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = w[4] + w[3] + w[1] + w[0];  /* a(i-1)+a(i-2)+a(i-4)+a(i-5) */

        /* Shift the window: drop the oldest, bring in the new term */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = next;
    }

    return w[4]; /* a(n) */
}
