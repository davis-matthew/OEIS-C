/*
 * A116773  Number of permutations of length n which avoid the patterns 1432,
 *          2134, 4132; or avoid the patterns 3124, 4123, 4321.
 * https://oeis.org/A116773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the rational generating function):
   a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 21, a(5) = 73, a(6) = 241.
   For n > 6:
   a(n) = 9*a(n‑1) - 32*a(n‑2) + 57*a(n‑3) - 53*a(n‑4) + 24*a(n‑5) - 4*a(n‑6) */
seq_t a(idx_t n)
{
    static const seq_t init[6] = {1,2,6,21,73,241};
    if (n <= 6)
        return init[n - 1];

    seq_t v[6];
    for (int k = 0; k < 6; ++k)
        v[k] = init[k];

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t sumA = 9 * v[5] + 57 * v[3] + 24 * v[1];
        seq_t sumB = 32 * v[4] + 53 * v[2] + 4 * v[0];
        seq_t next   = sumA - sumB;
        /* shift window left */
        for (int k = 0; k < 5; ++k)
            v[k] = v[k + 1];
        v[5] = next;
    }
    return v[5];
}
