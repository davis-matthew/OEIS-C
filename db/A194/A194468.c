/*
 * A194468  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) > 0, where
 *          r=sqrt(3) and c=sqrt(1/3), and < > denotes fractional part.
 * https://oeis.org/A194468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194468
 *  a(n) = the nth integer m for which
 *         m^2 > 3 * T(m)^2
 *  where  T(m) = |{ k ≤ m : frac(k·sqrt(3)) ≥ 1 - 1/√3 }|
 *  The test D(k)=1 if:
 *  3·(floor(k·sqrt(3))+1)^2 ≤ (3k+1)^2
 *  (floor(k·sqrt(3)) is computed as floor(sqrt(3k²))).
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root (floor) for 64‑bit unsigned integers */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0;
    uint64_t bit = (uint64_t)1ULL << 62;          /* highest power of 4 ≤ 2^64-1 */

    while (bit > n) bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (n >= x + bit) {
            n   -= x + bit;
            x   = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
    }
    return x;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms  = NULL;
    static size_t   len   = 0;   /* number of terms stored */
    static size_t   cap   = 0;
    static idx_t   last_m = 0;   /* last m processed */
    static uint64_t T     = 0;   /* cumulative sum of D(k) */

    /* grow array if necessary */
    if (len == cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* memory exhaustion */
        terms = tmp;
        cap   = new_cap;
    }

    while (len < (size_t)n) {
        ++last_m;
        uint64_t k = (uint64_t)last_m;

        /* compute D(k) */
        unsigned __int128 prod = (unsigned __int128)k * k * 3;   /* 3·k² */
        uint64_t sq = (uint64_t)prod;                           /* fits in u64 for k in range */
        uint64_t mfloor   = isqrt_u64(sq);                      /* floor(k·sqrt3) = floor(sqrt(3k²)) */
        uint64_t B        = mfloor + 1;                         /* m + 1 */
        uint64_t L        = 3ULL * B * B;                      /* left side of inequality */
        uint64_t R        = (3ULL * k + 1ULL) * (3ULL * k + 1ULL);  /* right side of inequality */

        uint64_t D = (R >= L) ? 1 : 0;
        T += D;               /* update cumulative sum */

        /* test m² > 3·T² */
        unsigned __int128 lhs = (unsigned __int128)k * k;
        unsigned __int128 rhs = (unsigned __int128)3 * T * T;
        if (lhs > rhs) {
            terms[len++] = (seq_t)last_m;  /* store term */
        }
    }

    return terms[n - 1];
}
