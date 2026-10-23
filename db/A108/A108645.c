/*
 * A108645  a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(2*n^2 + 6*n + 5)/720.
 * https://oeis.org/A108645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(2*n^2 + 6*n + 5)/720
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 532, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    // For C++ enables compile‑time branching on the size of seq_t.
    if constexpr (sizeof(seq_t) <= 8) {               /* primitive types (≤ 64‑bit) */
        unsigned __int128 prod = 1;
        prod *= (n + 1);
        prod *= (n + 2);
        prod *= (n + 2);
        prod *= (n + 3);
        prod *= (n + 3);
        prod *= (n + 4);
        prod *= ((unsigned __int128)2 * n * n + (unsigned __int128)6 * n + 5);
        prod /= 720;
        return (seq_t)prod;
    } else {                                         /* arbitrary‑precision types */
        seq_t prod = 1;
        prod *= (seq_t)(n + 1);
        prod *= (seq_t)(n + 2);
        prod *= (seq_t)(n + 2);
        prod *= (seq_t)(n + 3);
        prod *= (seq_t)(n + 3);
        prod *= (seq_t)(n + 4);
        prod *= ((seq_t)2 * n * n + (seq_t)6 * n + 5);
        prod /= 720;
        return prod;
    }
#else
    /* C standard solution: use unsigned __int128 for intermediate precision */
    unsigned __int128 prod = 1;
    prod *= (n + 1);
    prod *= (n + 2);
    prod *= (n + 2);
    prod *= (n + 3);
    prod *= (n + 3);
    prod *= (n + 4);
    prod *= ((unsigned __int128)2 * n * n + (unsigned __int128)6 * n + 5);
    prod /= 720;
    return (seq_t)prod;
#endif
}
