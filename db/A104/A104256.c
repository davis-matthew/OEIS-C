/*
 * A104256  [ (2n-1)!!/(4n) ].
 * https://oeis.org/A104256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2n-1)!!/(4*n))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (2n-1)!! = product_{k=1..n}(2k-1)   */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but keep safe */

    /* Use a larger intermediate type to avoid overflow on 64‑bit builds.
       unsigned __int128 is available on all supported compilers. */
    unsigned __int128 prod = 1;

    for (idx_t k = 1; k <= n; ++k) {
        prod *= (unsigned long long)(2 * k - 1);
    }

    /* divide by 4*n (exact floor division) */
    prod /= ((unsigned __int128)4 * n);

    return (seq_t)prod;
}
