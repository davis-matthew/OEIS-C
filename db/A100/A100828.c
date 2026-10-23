/*
 * A100828  Expansion of (1+2*x-2*x^3-3*x^2)/((x-1)*(x+1)*(x^2+2*x-1)).
 * https://oeis.org/A100828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) for n > 3 with
 *            initial values a(0)=1, a(1)=4, a(2)=7, a(3)=18
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)18;

    seq_t a0 = (seq_t)1;  // a(n‑4)
    seq_t a1 = (seq_t)4;  // a(n‑3)
    seq_t a2 = (seq_t)7;  // a(n‑2)
    seq_t a3 = (seq_t)18; // a(n‑1)

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = a3 + a3;   // 2*a(n‑1)
        new_val += a2 + a2;        // +2*a(n‑2)
        new_val -= a1 + a1;        // -2*a(n‑3)
        new_val -= a0;             // -a(n‑4)

        // Slide window for next iteration
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3;
}
