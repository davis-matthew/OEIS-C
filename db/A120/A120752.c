/*
 * A120752  Numbers k such that {rk} <= c, where r = (1/2)^(1/2), c = 1/2 and
 *          { } denotes fractional part.
 * https://oeis.org/A120752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that {k*(1/√2)} ≤ 1/2, equivalently
   floor(k*sqrt(2)) is even. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned __int128 using binary search. */
static inline unsigned __int128 isqrt128(unsigned __int128 n)
{
    if (n == 0u) return 0u;
    unsigned __int128 low = 1u, high = (unsigned __int128)1 << 64;  /* 2^64 > sqrt(max 128‑bit)  */
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid <= n / mid)           /* mid*mid ≤ n */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                      /* floor(sqrt(n)) */
}

/*  Sequential search for the nth term. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;      /* undefined; return zero for safety */
    idx_t count = 0u;
    for (idx_t k = 1u; ; ++k) {
        unsigned __int128 kk = (unsigned __int128)k * k;   /* k² */
        unsigned __int128 t  = isqrt128(2u * kk);          /* floor(k*sqrt(2)) */
        if ((t & 1u) == 0u) {                             /* even? */
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
    /* unreachable */
    return (seq_t)0;
}
