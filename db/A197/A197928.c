/*
 * A197928  Number of pairs of integers i,j with 1<=i<=n, 1<=j<=i, such that
 *          i^2-j^2 = i*j (mod n).
 * https://oeis.org/A197928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (i,j) : 1 ≤ j ≤ i ≤ n, * i^2 - j^2 ≡ i*j (mod n) }
 * Range:     u8 n <= 208, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;          /* index 0 is unused, return 0 */
    seq_t N = n;                   /* convert n to seq_t for modulo ops */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* i mod N – i ≡ 0 only when i == N */
        seq_t i_mod = (i < N) ? i : 0;
        seq_t i_sq  = (i_mod * i_mod) % N;  /* i^2 mod N */

        for (idx_t j = 1; j <= i; ++j)
        {
            seq_t j_mod = (j < N) ? j : 0;
            seq_t j_sq  = (j_mod * j_mod) % N;       /* j^2 mod N */
            seq_t ij    = (i_mod * j_mod) % N;       /* i*j mod N */

            /* Compute (i^2 - j^2 - i*j) mod N safely   */
            seq_t expr = (i_sq + N - j_sq) % N;      /* i^2 - j^2 (mod N) */
            expr = (expr + N - ij) % N;             /* subtract i*j (mod N) */

            if (expr == 0)
                ++count;
        }
    }
    return count;
}
