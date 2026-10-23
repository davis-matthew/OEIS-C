/*
 * A115566  Numbers k such that 2^k, 2^(k+1) and 2^(k+2) have the same number
 *          of digits.
 * https://oeis.org/A115566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that fractional part of
 *            (k·log10(2)) is < 1 – 2·log10(2)
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Scaling constants for 10^18 fixed‑point arithmetic.
   log10(2) ≈ 0.301029995663981195213738894724...   */
static const unsigned long long SCALE   = 1000000000000000000ULL; /* 10^18 */
static const unsigned long long LOG2    = 301029995663981195ULL;   /* floor(log10(2)*SCALE) */
static const unsigned long long THRESH = 397940008672037610ULL;   /* floor((1-2·log10(2))*SCALE) */

/* Check whether a given k satisfies the condition. */
static int k_is_valid(unsigned long long k)
{
    /* Compute the fractional part of k*log10(2) in fixed point. */
    unsigned __int128 prod = (unsigned __int128)k * LOG2;
    unsigned long long frac = (unsigned long long)(prod % (unsigned __int128)SCALE);

    /* The digits of 2^k, 2^(k+1), 2^(k+2) are equal iff
       frac < THRESH. */
    return frac < THRESH;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n=0, but safe fallback. */

    idx_t count = 0;
    unsigned long long k = 1;

    while (1) {
        if (k_is_valid(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
