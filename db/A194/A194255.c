/*
 * A194255  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=4-sqrt(12).
 * https://oeis.org/A194255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ{k=1..n} { (4‑√12)·k } ), * where {x} = x – ⌊x⌋
 *            is the fractional part. * This can be rewritten as * * a(n) = n
 *            + Σ{k=1..n} ⌊ 2·√3 ·k ⌋ – ⌈ √3 ·n·(n+1) ⌉ * * The sums are
 *            evaluated using integer arithmetic only
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety: sequence starts at n=1 */
    seq_t S = 0;                 /* S = Σ floor(2·√3·k)   */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t nk = (seq_t)k;
        seq_t val = floor_sqrt(nk * nk * 12);   /* floor( √(12·k²) ) = floor(2·√3·k) */
        S += val;
    }

    /* Compute ceil( √3·n·(n+1) ) using integer square root   */
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;
    seq_t np1  = n_seq + 1;
    seq_t np12 = np1 * np1;

    /* Use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
    unsigned __int128 temp = (unsigned __int128)n2 * (unsigned __int128)np12 * 3;
    seq_t X = (seq_t)temp;                    /* X = 3·n²·(n+1)² = (√3·n·(n+1))² */

    seq_t y = floor_sqrt(X);                  /* floor of sqrt(X)   */
    seq_t ceil_sqrt = (y * y == X) ? y : y + 1;

    seq_t result = n_seq + S - ceil_sqrt;     /* a(n) = n + S – ceil_sqrt */
    return result;
}
