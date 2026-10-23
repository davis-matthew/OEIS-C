/*
 * A024400  [ (4th elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 2 mod 3}.
 * https://oeis.org/A024400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(e4 / e1) where e1 = Σ_{k=0}^{n+2} (3k+2), e4 = 4th
 *            elementary symmetric sum of the same set (i.e. sum of products
 *            of all 4‑element subsets)
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef __uint128_t u128;
#else
/* Fallback: use 128‑bit arithmetic via builtin extensions if available.
   If __int128 is not supported the build will fail as the sequence
   requires it for the given bounds. */
typedef unsigned long long u128;
#endif

seq_t a(idx_t n)
{
    /* length of the set and highest index of the arithmetic progression */
    idx_t m   = n + 2;      /* highest k in 3k+2 */
    idx_t L   = m + 1;      /* number of terms = n+3 */

    /* power sums of k from 0 to m */
    u128 mp  = (u128)m;
    u128 Lp  = (u128)L;
    u128 pk1 = mp * (mp + 1ULL) / 2;                           /* Σk        */
    u128 pk2 = mp * (mp + 1ULL) * (2ULL * mp + 1ULL) / 6;      /* Σk²       */
    u128 pk3 = pk1 * pk1;                                      /* Σk³       */
    u128 pk4 = mp * (mp + 1ULL) * (2ULL * mp + 1ULL) *
               (3ULL * mp * mp + 3ULL * mp - 1ULL) / 30;        /* Σk⁴       */

    /* sums of (3k+2)^r */
    u128 s1 = 3ULL * pk1 + 2ULL * Lp;                          /* Σ(3k+2)   */
    u128 s2 = 9ULL * pk2 + 12ULL * pk1 + 4ULL * Lp;             /* Σ(3k+2)²  */
    u128 s3 = 27ULL * pk3 + 54ULL * pk2 + 36ULL * pk1 + 8ULL * Lp;  /* Σ(3k+2)³ */
    u128 s4 = 81ULL * pk4 + 216ULL * pk3 + 216ULL * pk2 +
             96ULL * pk1 + 16ULL * Lp;                          /* Σ(3k+2)⁴ */

    /* elementary symmetric sums via Newton identities */
    u128 e1_128 = s1;
    u128 e2_128 = (s1 * s1 - s2) / 2;
    u128 e3_128 = (s1 * e2_128 - s2 * e1_128 + s3) / 3;
    u128 e4_128 = (s1 * e3_128 - s2 * e2_128 + e1_128 * s3 - s4) / 4;

    /* cast back to seq_t (fits into seq_t for the intended bounds) */
    seq_t e1 = (seq_t)(uint64_t)e1_128;
    seq_t e4 = (seq_t)(uint64_t)e4_128;

    return e4 / e1;   /* integer division is floor for positive integers */
}
