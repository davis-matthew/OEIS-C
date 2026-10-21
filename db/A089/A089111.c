/*
 * A089111  Convoluted convolved Fibonacci numbers G_6^(r).
 * https://oeis.org/A089111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4a(n-1) -6a(n-2) +4a(n-3) - a(n-4) + a(n-5) -4a(n-6)
 *            +6a(n-7) -4a(n-8) + a(n-9) for n ≥ 10 with a(1)=8, a(2)=19,
 *            a(3)=37, a(4)=64, a(5)=102, a(6)=154, a(7)=222, a(8)=309,
 *            a(9)=418
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 839, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = { 8ull, 19ull, 37ull, 64ull, 102ull, 154ull, 222ull, 309ull, 418ull };
    if (n <= 9) return init[n - 1];

    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) buf[i] = init[i];
    seq_t cur = 0;

    for (idx_t m = 10; m <= n; ++m)
    {
        /* compute positive part: 4·a(n‑1) + 4·a(n‑3) + a(n‑5) + 6·a(n‑7) + a(n‑9) */
        seq_t posSum = buf[8] * (seq_t)4 + buf[6] * (seq_t)4 + buf[4] + buf[2] * (seq_t)6 + buf[0];
        /* compute negative part: 6·a(n‑2) + a(n‑4) + 4·a(n‑6) + 4·a(n‑8) */
        seq_t negSum = buf[7] * (seq_t)6 + buf[5] + buf[3] * (seq_t)4 + buf[1] * (seq_t)4;

        cur = posSum;
        cur -= negSum;          /* final subtraction; result is non‑negative */

        /* roll the history buffer */
        for (idx_t i = 0; i < 8; ++i) buf[i] = buf[i + 1];
        buf[8] = cur;
    }
    return cur;
}
