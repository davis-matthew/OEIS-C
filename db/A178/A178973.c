/*
 * A178973  Number of ways to place 3 nonattacking amazons (superqueens) on an
 *          n X n toroidal board.
 * https://oeis.org/A178973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * ( 2*n^4 - 24*n^3 + 61*n^2 + 168*n - 570 + (3*n^2 -
 *            24*n + 42)*(-1)^n ) / 12 (valid for n >= 7; a(n) = 0 for 1 <= n
 *            <= 6)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;

    /* use 64-bit signed arithmetic for the intermediate polynomial */
    int64_t n64   = (int64_t)n;
    int64_t n_sq  = n64 * n64;          /* n^2   */
    int64_t n_cub = n_sq  * n64;        /* n^3   */
    int64_t n_four= n_cub * n64;        /* n^4   */

    /* base = 2*n^4 - 24*n^3 + 61*n^2 + 168*n - 570          */
    int64_t base = 2 * n_four - 24 * n_cub + 61 * n_sq
                 + 168 * n64 - 570;

    /* term6 = 3*n^2 - 24*n + 42                           */
    int64_t term6 = 3 * n_sq - 24 * n64 + 42;

    if ((n64 & 1) == 0)   /* n even: add term6 */
        base += term6;
    else                  /* n odd : subtract term6 */
        base -= term6;

    /* a(n) = n^2 * base / 12                               */
    /* use 128‑bit intermediate when seq_t is small enough   */
    if (sizeof(seq_t) <= 8) {                /* 64‑bit or smaller */
        unsigned __int128 prod = (unsigned __int128)n_sq * (unsigned __int128)base;
        unsigned __int128 res  = prod / 12;
        return (seq_t)res;
    } else {                                 /* arbitrary or 128‑bit seq_t */
        seq_t prod = (seq_t)n_sq * (seq_t)base;
        return prod / 12;
    }
}
