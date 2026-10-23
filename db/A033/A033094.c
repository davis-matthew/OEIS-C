/*
 * A033094  Number of 0's when k is written in base b for all b and k
 *          satisfying 2<=b<=n+1, 1<=k<=n.
 * https://oeis.org/A033094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{b=2}^{n+1} ∑_{p≥0, b^p ≤ n} [⌊(n+1)/b^{p+1}⌋·b^p +
 *            min((n+1) mod b^{p+1}, b^p) – b^p]
 * Range:     u8 n <= 46, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of zeros in the base‑b representations of all k (1 ≤ k ≤ n) for every b. */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t b = 2; b <= n + 1; ++b)
    {
        idx_t power = 1;                     /* power = b^p   */
        while (power <= n)
        {
            idx_t block = power * b;          /* block = b^(p+1) */

            /* Count numbers in [0, n] whose digit at position p is 0 */
            idx_t full_blocks = (n + 1) / block;
            seq_t zeros_f = (seq_t)full_blocks * power;

            idx_t remaining = (n + 1) % block;
            seq_t add = (remaining < power) ? remaining : power;
            zeros_f += add;                    /* total zeros at position p */

            /* Exclude numbers with leading digit at position p (k < b^(p+1)) */
            seq_t zeros_per_position = zeros_f - power;

            sum += zeros_per_position;

            power *= b;                       /* move to next position */
        }
    }

    return sum;
}
