/*
 * A152104  Quartic product sequence: a(n) = 2^n*Product_{k=1..(n-1)/2} (1 +
 *          m*cos(k*Pi/n)^2 + q*cos(k*Pi/n)^4), with m=6, q=4.
 * https://oeis.org/A152104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * Π_{k=1..(n-1)/2} (1 + 6*cos(kπ/n)^2 +
 *            4*cos(kπ/n)^4) Recurrence (valid for n ≥ 5): a(n) = 2*a(n‑1) +
 *            8*a(n‑2) + 2*a(n‑3) – a(n‑4) Initial values: a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=22, a(4)=80
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0 … 4 */
    static const seq_t base[5] = {1, 2, 4, 22, 80};

    if (n <= 4)
        return base[n];

    /* iterate the recurrence */
    seq_t prev4 = base[1];   /* a(n-4) */
    seq_t prev3 = base[2];   /* a(n-3) */
    seq_t prev2 = base[3];   /* a(n-2) */
    seq_t prev1 = base[4];   /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 8*a(i-2) + 2*a(i-3) - a(i-4) */
        next = prev1 * 2 + prev2 * 8 + prev3 * 2 - prev4;

        /* shift the window for the next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
