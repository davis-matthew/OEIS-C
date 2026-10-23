/*
 * A187174  Number of 4-step left-handed knight's tours (moves only out two,
 *          left one) on an n X n board summed over all starting positions.
 * https://oeis.org/A187174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*n^2 - 260*n + 440 for n > 5 a(1) = 0, a(2) = 0, a(3) =
 *            0, a(4) = 8, a(5) = 48
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small n exactly */
    if (n <= 5)
    {
        switch ((unsigned)n)
        {
            case 1: return 0;
            case 2: return 0;
            case 3: return 0;
            case 4: return 8;
            case 5: return 48;
            default: return 0; /* for n == 0 */
        }
    }

    /* For n > 5 use the quadratic formula */
    seq_t N = (seq_t)n;
    return 36*N*N - 260*N + 440;
}
