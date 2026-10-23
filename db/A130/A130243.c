/*
 * A130243  Partial sums of the 'lower' Lucas Inverse A130241.
 * https://oeis.org/A130243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (# Lucas numbers ≤ k) Equivalent: let c(n) =
 *            # Lucas numbers ≤ n, s(n) = sum of those c(n) Lucas numbers.
 *            Then a(n) = (n+1)·c(n) – s(n). This implementation computes c(n)
 *            and s(n) on the fly
 * Range:     u8 n <= 45, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;     /* number of Lucas numbers ≤ n   */
    seq_t sum   = 0;     /* sum of those Lucas numbers   */

    /* Lucas recurrence: L0 = 2, L1 = 1, L(k+2) = L(k+1)+L(k)   */
    seq_t L_prev = 2;    /* L0   */
    seq_t L_curr = 1;    /* L1   */

    /* Iterate over L1, L2, L3, … while they are ≤ n */
    while (L_curr <= (seq_t)n)
    {
        ++count;
        sum += L_curr;

        seq_t L_next = L_prev + L_curr;
        L_prev = L_curr;
        L_curr = L_next;
    }

    seq_t n_plus1 = (seq_t)n + 1;
    seq_t result = n_plus1 * count - sum;

    return result;
}
