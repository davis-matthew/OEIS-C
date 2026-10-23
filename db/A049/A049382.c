/*
 * A049382  Expansion of (1-25*x)^(-4/5).
 * https://oeis.org/A049382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(5*n - 1)/n * a(n-1), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(25 * i - 5);   /* (25*i - 5) = 5*(5*i - 1) */

#if defined(__SIZEOF_INT128__)        /* use 128‑bit intermediate when available */
        unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)num;
        val = (seq_t)(prod / i);
#elif defined(_MSC_VER)               /* MSVC fallback using 128‑bit via __int128 */
        unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)num;
        val = (seq_t)(prod / i);
#else                                 /* simple 64‑bit product; may overflow if seq_t is 64‑bit */
        unsigned long long prod = (unsigned long long)val * (unsigned long long)num;
        val = (seq_t)(prod / i);
#endif
    }
    return val;
}
