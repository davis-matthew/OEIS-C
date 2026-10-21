/*
 * A216066  a(n) = card {cos((2^k)*Pi/(2*n-1)): k in N}.
 * https://oeis.org/A216066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal r >= 1 such that 2^r ≡ ±1 (mod 2*n-1), with
 *            a(1)=1
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t m = (seq_t)n * 2;
    m -= 1;                       /* m = 2*n - 1 (odd) */

    seq_t val = 2 % m;             /* first power, 2^1 */
    seq_t r   = 1;

    if (val == 1 || val == m - 1) return r;

    while (1) {
        /* safe modular multiplication by 2 using 128‑bit arithmetic */
        val = (seq_t)((unsigned __int128)val * 2 % (unsigned __int128)m);
        ++r;
        if (val == 1 || val == m - 1) break;
    }
    return r;
}
