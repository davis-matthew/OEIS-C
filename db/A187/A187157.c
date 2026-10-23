/*
 * A187157  Number of 4-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*n^2 - 168*n + 188 for n > 2; a(1)=0, a(2)=0
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    /* use 64‑bit arithmetic for the intermediate computation */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long term = 36ULL * nn * nn
                            - 168ULL * nn
                            + 188ULL;
    return (seq_t)term;
}
