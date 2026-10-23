/*
 * A194380  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(7) and < > denotes fractional part.
 * https://oeis.org/A194380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A194380: Numbers m such that
 *   Σ_{k=1..m} (〈½ + k√7〉 - 〈k√7〉) > 0
 *
 *  The term is found by iterating k, testing whether
 *  〈k√7〉 < ½.  This is equivalent to
 *     ⌊√(28k²)⌋ = 2·⌊√(7k²)⌋
 *  which is evaluated using only integer arithmetic.
 *  The sum of +½ and –½ terms is tracked in a signed
 *  difference counter; a term is recorded whenever this
 *  counter becomes positive.
 *
 *  The function returns the nth such m (1‑based).  No caching
 *  is used; each call recomputes the sequence up to the
 *  requested index, which is fast enough for n ≤ 10000.
 */
#include <stddef.h>
#include <stdint.h>

/* --- integer square root for unsigned __int128 -------------------------------- */
static inline seq_t isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = ~0ULL;          /* 2⁶⁴‑1 */
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return (seq_t)(lo - 1);
}

/* --- main function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but harmless */

    idx_t      k        = 1;          /* current index k */
    int64_t    diff     = 0;          /* (# of +½ terms) – (# of –½ terms) */
    idx_t      counted  = 0;          /* how many m's found so far */

    for (;;) {
        /* compute floor(k√7) and floor(2k√7) without floating‑point */
        unsigned __int128 prod7  = (unsigned __int128)7 * k * k;
        seq_t  a_val = isqrt128(prod7);

        unsigned __int128 prod28 = (unsigned __int128)28 * k * k;
        seq_t  b_val = isqrt128(prod28);

        /* sign: +1 if fractional part of k√7 < ½, else –1 */
        if (b_val == a_val + a_val) diff += 1;
        else                        diff -= 1;

        if (diff > 0) {                /* m satisfies the inequality   */
            counted++;
            if (counted == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
