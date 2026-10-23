/*
 * A188150  Number of 5-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1,2; a(3) = 104; a(n) = 100*n^2 - 360*n + 272
 *            for n ≥ 4 (derived from the empirical quadratic fit for n > 3)
 *            The recurrence a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) also holds
 *            for n > 6
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
    {
        switch ((idx_t)n)
        {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)104;
            default: return (seq_t)0; /* unreachable */
        }
    }

    seq_t nn = (seq_t)n;
    /* use 128‑bit intermediate to avoid overflow for large n */
    __int128 val = (__int128)100 * (__int128)nn * (__int128)nn;
    val -= (__int128)360 * (__int128)nn;
    val += 272;
    return (seq_t)val;
}
