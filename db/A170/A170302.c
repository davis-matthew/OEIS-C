/*
 * A170302  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 5·4^(n‑1) for 1 ≤ n ≤ 41 a(n) =
 *            (1/2)·(a(n‑1)+a(n‑2)+…+a(n‑41)) for n ≥ 42 (derived from the
 *            rational GF in the OEIS entry) The recurrence uses a sliding
 *            window of the last 41 values
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial first term. */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute the first 42 terms, the first 41 of which are a geometric
       progression (5·4^(n‑1)).  They are needed to seed the recurrence.   */
    seq_t init[42];
    init[0] = (seq_t)1;
    init[1] = (seq_t)5;
    for (idx_t i = 2; i <= 41; ++i)
        init[i] = init[i - 1] * (seq_t)4;

    /* If the requested index is within the pre‑computed range, return it. */
    if (n <= 41)
        return init[n];

    /* Sliding window of the last 41 values.  The window contains
       init[1] .. init[41] after the initialisation.                */
    seq_t last[41];
    for (idx_t i = 1; i <= 41; ++i)
        last[i - 1] = init[i];

    /* Sum of the values currently in the window – used to compute the next term. */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < 41; ++i)
        sum += last[i];

    idx_t idx = 0;          /* position of the oldest value in the circular buffer */
    seq_t result = 0;       /* value of the current term a(n) */

    /* Generate terms from n=42 up to the requested index. */
    for (idx_t cur = 42; cur <= n; ++cur) {
        result = sum / (seq_t)2;           /* recurrence: a(n) = (1/2)*sum of last 41 */

        /* Update the sliding window: discard the oldest, insert the new value. */
        sum -= last[idx];
        last[idx] = result;
        sum += result;

        idx++;
        if (idx == 41)
            idx = 0;
    }

    return result;
}
