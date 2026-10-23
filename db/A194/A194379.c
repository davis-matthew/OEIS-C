/*
 * A194379  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(7) and < > denotes fractional part.
 * https://oeis.org/A194379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A194379
    a(n) = smallest even m such that
           #{k : 1≤k≤m, frac(k*sqrt(7)) < 1/2} = m/2
    It is equivalent to the definition given in the OEIS entry. */
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static inline uint64_t floor_sqrt_uint128(unsigned __int128 v)
{
    /* Utility: floor(sqrt(v)) for 128‑bit unsigned integers. */
    unsigned long long x = (unsigned long long)sqrt((long double)v);
    while ((unsigned __int128)(x + 1) * (x + 1) <= v) ++x;
    while ((unsigned __int128)x * x > v) --x;
    return x;
}

seq_t a(idx_t n)
{
    idx_t cnt_terms = 0;          /* number of seen sequence terms   */
    idx_t k = 0;                  /* current integer 1..∞           */
    idx_t less_cnt = 0;           /* #{k’ ≤ k : frac(k’*sqrt7) < 0.5} */
    seq_t last = 0;               /* last found term                  */

    while (cnt_terms < n) {
        ++k;
        /* m = k.  Compute t = floor(k*sqrt(7)) = floor(sqrt(7*k^2)). */
        unsigned __int128 kk = (unsigned __int128)k;
        unsigned __int128 val = 7 * kk * kk;
        uint64_t t = floor_sqrt_uint128(val);

        /* Condition: 28*k^2 < 4*t^2 + 4*t + 1  ⇔  frac(k*sqrt7) < 0.5 */
        unsigned __int128 left  = 28 * kk * kk;
        unsigned __int128 right = 4 * (unsigned __int128)t * t
                                + 4 * (unsigned __int128)t + 1;

        if (left < right)
            ++less_cnt;               /* current k satisfies the < ½ test */

        /* m must be even.  Check the balancing condition. */
        if ((k & 1ULL) == 0) {
            if (less_cnt == k / 2) {
                ++cnt_terms;
                last = (seq_t)k;
            }
        }
    }
    return last;
}
