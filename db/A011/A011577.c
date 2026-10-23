/*
 * A011577  Stirling numbers of second kind S2(28,n).
 * https://oeis.org/A011577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S(28, n), the Stirling number of the second kind for n =
 *            1 … 28
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    #include <stdbool.h>

    static seq_t cache[29];
    static bool initialized = false;

    if (!initialized)
    {
        seq_t cur[29] = {0};
        cur[0] = (seq_t)1;   /* S(0,0) = 1 */

        for (idx_t i = 1; i <= 28; ++i)
        {
            for (idx_t k = i; k > 0; --k)
                cur[k] = (seq_t)k * cur[k] + cur[k-1];
            cur[0] = (seq_t)0;   /* S(i,0) = 0 for i > 0 */
        }

        for (idx_t k = 1; k <= 28; ++k)
            cache[k] = cur[k];

        initialized = true;
    }

    return (n >= 1 && n <= 28) ? cache[n] : (seq_t)0;
}
