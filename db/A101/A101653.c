/*
 * A101653  a(n)=Product{k=0..n, 1+3^A010060(k)}/2.
 * https://oeis.org/A101653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{k=0..n} (1+3^A010060(k))) / 2 where A010060(k) is the
 *            Thue–Morse sequence (parity of popcount(k)). The recurrence
 *            simplifies to: a(0) = 1 a(n) = a(n-1) * (1 + 3^tm(n)) for n >= 1
 *            Since tm(n) ∈ {0,1}, the factor is 2 when tm(n)=0 and 4 when
 *            tm(n)=1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* Determine the Thue–Morse value: 1 if popcount(k) is odd, else 0. */
        if ((__builtin_popcountll((unsigned long long)k) & 1) != 0)
            result *= (seq_t)4;   /* 1 + 3^1 = 4 */
        else
            result *= (seq_t)2;   /* 1 + 3^0 = 2 */
    }
    return result;
}
