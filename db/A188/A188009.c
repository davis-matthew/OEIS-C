/*
 * A188009  a(n) = [nr] - [nr-kr] - [kr], where r=(1+sqrt(5))/2, k=2, [
 *          ]=floor.
 * https://oeis.org/A188009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) – floor(n*r – 2*r) – floor(2*r), r =
 *            (1+sqrt(5))/2 The implementation avoids floating‑point
 *            arithmetic by using a fixed‑point scaling of sqrt(5). The
 *            scaling factor is 2^60, giving an error far below the minimum
 *            possible distance of n*phi to an integer for n ≤ 10000
 * Range:     u8 n <= 136, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* -----------------------------------------------------------------------
   Precompute a 60‑bit fixed‑point representation of sqrt(5):
       sqrt5_fixed = floor(sqrt(5) * 2^60)
   We compute this by integer square‑root on the 128‑bit number 5 * 2^120.
   ----------------------------------------------------------------------- */
static uint64_t sqrt5_fixed;   /* value = floor(sqrt(5) * 2^60) */
static int sqrt5_ready = 0;    /* flag to ensure single initialization */

static void init_sqrt5_fixed(void)
{
    if (sqrt5_ready) return;

    /* 5 * 2^120 fits in 128 bits */
    const unsigned __int128 target = ((unsigned __int128)5) << 120;

    unsigned long long lo = 0;
    unsigned long long hi = ((unsigned long long)1) << 63;   /* upper bound   */

    while (lo < hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= target)
            lo = mid + 1;
        else
            hi = mid;
    }
    sqrt5_fixed = lo - 1;          /* floor(sqrt(target)) = floor(sqrt(5) * 2^60) */
    sqrt5_ready = 1;
}

/* -----------------------------------------------------------------------
   Computes floor(m * phi)   (phi = (1+sqrt(5))/2) for a non‑negative integer m
   using the exact formula:
       floor(m*phi) = floor( (m + floor(m*sqrt(5))) / 2 )
   While floor(m*sqrt(5)) is obtained through the fixed‑point representation.
   ----------------------------------------------------------------------- */
static seq_t floor_phi(idx_t m)
{
    init_sqrt5_fixed();

    /* floor(m*sqrt(5)) = floor( (m * sqrt5_fixed) / 2^60 ) */
    unsigned __int128 prod = (unsigned __int128)m * sqrt5_fixed;
    uint64_t floor_sqrt5 = (uint64_t)(prod >> 60);

    /* (m + floor_sqrt5) / 2 , integer division truncates */
    uint64_t result = (m + floor_sqrt5) >> 1;
    return (seq_t)result;
}

/* -----------------------------------------------------------------------
   The sequence A188009: a(n) = floor(n*phi) – floor((n-2)*phi) – 3
   For n ≤ 4 the definition uses negative arguments; those values are all zero.
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 4) return (seq_t)0;

    seq_t fn  = floor_phi(n);
    seq_t fn2 = floor_phi(n - 2);

    return fn - fn2 - (seq_t)3;
}
