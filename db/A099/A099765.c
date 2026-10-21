/*
 * A099765  a(n) = (1/Pi) * (2^n/n) * (n-1)! * Integral_{t>=0} (sin(t)/t)^n
 *          dt.
 * https://oeis.org/A099765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=0..floor(n/2)} (-1)^k * binomial(n,k) *
 *            (n-2*k)^(n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers for integer operations */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* pow_uint(base,exp) -> seq_t */

/* --- Small integer build (uint8_t … uint64_t) ------------------------------------ */
static inline unsigned __int128 pow_uint128(uint64_t base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

static inline seq_t a_small(idx_t n)
{
    if (n < 1) return 0;
    unsigned __int128 sum_even = 0, sum_odd = 0;
    unsigned __int128 current  = 1;          /* binomial coefficient C(n,k) */
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        uint64_t base  = n - 2 * k;
        unsigned __int128 power = pow_uint128(base, n - 1);
        unsigned __int128 term  = current * power;
        if ((k & 1) == 0) sum_even += term;
        else              sum_odd  += term;
        if (k != maxk) {
            current = current * (n - k) / (k + 1);   /* next binomial C(n,k+1) */
        }
    }
    unsigned __int128 diff   = sum_even - sum_odd;
    unsigned __int128 result = diff / n;
    return (seq_t)result;
}

/* --- Big integer build (arbitrary precision) ----------------------------------- */
static inline seq_t a_big(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    seq_t current  = 1;          /* binomial coefficient C(n,k) */
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        uint64_t base = n - 2 * k;
        seq_t power   = pow_uint(base, n - 1);             /* (n-2k)^(n-1) */
        seq_t term    = current * power;
        if ((k & 1) == 0) sum_even += term;
        else              sum_odd  += term;
        if (k != maxk) {
            current = current * (seq_t)(n - k) / (seq_t)(k + 1);   /* C(n,k+1) */
        }
    }
    seq_t diff    = sum_even - sum_odd;
    seq_t result  = diff / (seq_t)n;
    return result;
}

/* ------------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Pick the appropriate routine depending on the size of seq_t. */
    if (sizeof(seq_t) <= 8) {
        return a_small(n);
    } else {
        return a_big(n);
    }
}
