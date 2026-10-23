/*
 * A103621  Trajectory of 7 under repeated application of the map n ->
 *          A102370(n).
 * https://oeis.org/A103621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(1) + Σ_{i=1}^{n-1} c[(i-1) mod 8] where a(1)=7 and the
 *            difference cycle c = {2,2,2,10,2,2,34,10} repeats forever.
 *            Equivalently, a(n) satisfies a(n) = a(n-1) + a(n-8) - a(n-9) for
 *            n > 9
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* difference cycle */
    static const unsigned int cycle[8] = {2,2,2,10,2,2,34,10};

    /* number of increments after the first term */
    idx_t m = n - 1;

    /* full 8‑step blocks and remainder */
    uint64_t full   = m / 8;
    idx_t   rem     = m % 8;

    /* each full block increases the value by 64 */
    uint64_t sum = 7 + full * 64ULL;

    /* add the partial block */
    for (idx_t i = 0; i < rem; ++i)
        sum += cycle[i];

    return (seq_t)sum;
}
