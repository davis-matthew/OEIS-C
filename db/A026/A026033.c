/*
 * A026033  a(n) = C(4*n,n) - C(4*n,n-4).
 * https://oeis.org/A026033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4*n,n) – C(4*n,n-4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Multiplicative binomial coefficient.
   Works for any seq_t that supports +, -, *, /.
   For 64‑bit seq_t we use an __int128 intermediate to avoid overflow.
*/
static inline seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;

    /* use the smaller of K and N-K */
    if (K > N - K) K = N - K;

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        const uint64_t num = N - K + i;
        const uint64_t den = i;

        if (sizeof(seq_t) <= 8) {
            /* safe 128-bit intermediate for 64‑bit seq_t */
            unsigned __int128 prod = (unsigned __int128)res * num;
            res = (seq_t)(prod / den);
        } else {
            res = res * (seq_t)num;     /* big‑int or larger type */
            res = res / (seq_t)den;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    const seq_t total   = binom(4 * n, n);
    const seq_t diff    = (n >= 4) ? binom(4 * n, n - 4) : 0;
    return total - diff;
}
