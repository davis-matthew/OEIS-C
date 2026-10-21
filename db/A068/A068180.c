/*
 * A068180  (Product_{i=1..4} (x+i)) / (Product_{i=1..4} (x-i)) = Sum_{n>=1}
 *          a(n)/A067419(n)*x^n.
 * https://oeis.org/A068180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=25, a(3)=625, a(4)=11095. For n≥5: a(n)=25·a(n‑1) –
 *            210·a(n‑2) + 720·a(n‑3) – 864·a(n‑4) + (864 if n==5 else 0).
 *            This follows from the generating function of A068180
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)625;
    if (n == 4) return (seq_t)11095;

    /* previous four values: a_{i-3}, a_{i-2}, a_{i-1}, a_i */
    seq_t a_prev[4] = { (seq_t)1, (seq_t)25, (seq_t)625, (seq_t)11095 };
    const seq_t c1 = (seq_t)25;
    const seq_t c2 = (seq_t)210;
    const seq_t c3 = (seq_t)720;
    const seq_t c4 = (seq_t)864;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t term = c1 * a_prev[3];
        term -= c2 * a_prev[2];
        term += c3 * a_prev[1];
        term -= c4 * a_prev[0];
        if (i == 5) term += (seq_t)864;          /* b_5 = 864 */
        /* shift history */
        a_prev[0] = a_prev[1];
        a_prev[1] = a_prev[2];
        a_prev[2] = a_prev[3];
        a_prev[3] = term;
    }
    return a_prev[3];
}
