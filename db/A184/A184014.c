/*
 * A184014  n + floor(sqrt(e*n)); complement of A184015.
 * https://oeis.org/A184014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(e*n))
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  e ≈ 2.718281828459045235…  */
/*  floor(e*n) is computed exactly using a fixed‑point approximation */
/*      e ≈ floor(e*10⁧10)/10⁧10  with 10⁧10 = 10000000000 */
/*  sqrt is taken on the integer floor(e*n) */

#include <stdint.h>

/* 10^10 scaling constants for e */
static const unsigned long long E10   = 27182818284ULL;   /* floor(e * 10^10)  */
static const unsigned long long SCALE10 = 10000000000ULL; /* 10^10           */

/* Integer square root for an unsigned __int128 argument.
   Returns the largest x such that x*x <= value.              */
static uint64_t isqrt_u128(unsigned __int128 value)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t cand = r | (1ULL << i);
        unsigned __int128 sq = (unsigned __int128)cand * cand;
        if (sq <= value)
            r = cand;
    }
    return r;
}

/* Sequence a(n) = n + floor(sqrt(e*n)) */
seq_t a(idx_t n)
{
    /* Compute floor(e*n) exactly via fixed‑point multiplication.  */
    unsigned __int128 prod = (unsigned __int128)n * E10;
    uint64_t en_floor = (uint64_t)(prod / SCALE10);   /* floor(e*n)    */

    /* Compute floor(sqrt(floor(e*n))) */
    uint64_t sqrt_val = isqrt_u128((unsigned __int128)en_floor);

    /* Return n + sqrt_val, cast to seq_t.                     */
    return (seq_t)n + (seq_t)sqrt_val;
}
