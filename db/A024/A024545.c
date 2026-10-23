/*
 * A024545  a(n) = Sum_{k=1..n} floor( 1/{k/sqrt(2)} ), where {x} := x -
 *          floor(x).
 * https://oeis.org/A024545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor( 1 /{k/sqrt(2)} ), where {x} = x -
 *            floor(x)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses fixed‑point arithmetic with a 62‑bit
   resolution so that all operations are exact for n ≤ 10 000 and
   all intermediate values fit in 128 bit. */

#include <stdint.h>
#include <stdbool.h>

/* Number of fixed‑point bits chosen so that 2^63 < 2^64
   and 128‑bit arithmetic can hold the intermediate product. */
#define SCALE_BITS   62ULL
#define SCALE        (1ULL << SCALE_BITS)          /* 2^62   */
#define SC2          (SCALE << 1)                  /* 2^63   */
#define SC2MASK      (SC2 - 1)                     /* lower 63 bits */

/* 64‑bit integer representation of floor( sqrt(2) * SCALE ).  */
static uint64_t sqrt2_scaled = 0;
static bool sqrt2_init = false;

/* Compute sqrt2_scaled by binary search: find largest r
   with r² ≤ 2 * SCALE²  */
static void init_sqrt2_scaled(void)
{
    const unsigned int S = SCALE_BITS;
    __uint128_t target = ((unsigned __int128)2) << (2 * S);   /* 2 * SCALE² */
    __uint128_t lo = 0, hi = ((unsigned __int128)1) << (S + 1); /* 2 * SCALE */
    while (lo < hi) {
        __uint128_t mid = (lo + hi + 1) >> 1;
        __uint128_t mid2 = mid * mid;
        if (mid2 <= target)
            lo = mid;
        else
            hi = mid - 1;
    }
    sqrt2_scaled = (uint64_t)lo;
}

seq_t a(idx_t n)
{
    if (!sqrt2_init) {
        init_sqrt2_scaled();
        sqrt2_init = true;
    }

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute k * floor(sqrt(2) * SCALE) as 128‑bit product. */
        __uint128_t prod = (__uint128_t)k * sqrt2_scaled;

        /* Lower 63 bits give the fractional part of k/√2 expressed
           in fixed‑point with denominator SC2 = 2^63.  */
        uint64_t frac_num = (uint64_t)(prod & SC2MASK);

        /* {k/√2} ≈ frac_num / SC2  (> 0).  Compute floor(1/frac) = SC2 / frac_num.  */
        if (frac_num == 0) {
            /* Extremely unlikely: this would mean the irrational fractional part
               was represented as zero.  Skip to avoid divide‑by‑zero.  */
            continue;
        }
        uint64_t val = ((unsigned __int128)SC2) / frac_num;
        sum += (seq_t)val;
    }
    return sum;
}
