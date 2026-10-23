/*
 * A011750  Expansion of (1 + x^2)/(1 + x + x^2 + x^3 + x^5) mod 2.
 * https://oeis.org/A011750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in (1 + x^2)/(1 + x + x^2 + x^3 +
 *            x^5) over GF(2). Recurrence: a_0 = a_1 = a_2 = a_3 = a_4 = 1;
 *            For n ≥ 5: a_n = a_{n-1} ^ a_{n-2} ^ a_{n-3} ^ a_{n-5}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)1;

    /* Sliding window of the last five terms: a0…a4 */
    seq_t a0 = (seq_t)1;  /* a_{n-5}   */
    seq_t a1 = (seq_t)1;  /* a_{n-4}   */
    seq_t a2 = (seq_t)1;  /* a_{n-3}   */
    seq_t a3 = (seq_t)1;  /* a_{n-2}   */
    seq_t a4 = (seq_t)1;  /* a_{n-1}   */

    for (idx_t i = 5; i <= n; ++i) {
        /* a_i = a_{i-1} + a_{i-2} + a_{i-3} + a_{i-5} (mod 2) -> XOR */
        seq_t next = a4 ^ a3 ^ a2 ^ a0;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
