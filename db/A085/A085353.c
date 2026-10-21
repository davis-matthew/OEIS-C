/*
 * A085353  Expansion of (1-5*x)/((1-6*x)*(1-7*x)).
 * https://oeis.org/A085353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*7^n - 6^n Recurrence: a(0) = 1; a(n) = 7*a(n-1) +
 *            6^(n-1) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)1;        // a(0)
    seq_t pow6  = (seq_t)1;        // 6^(i-1), initial for i = 1

    for (idx_t i = 1; i <= n; ++i) {
        a_val = a_val * (seq_t)7 + pow6;   // a(i) = 7*a(i-1) + 6^(i-1)
        pow6  = pow6 * (seq_t)6;           // update 6^i for next iteration
    }
    return a_val;
}
