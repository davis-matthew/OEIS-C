/*
 * A072219  Any number n can be written uniquely in the form n = 2^k_1 - 2^k_2
 *          + 2^k_3 - ... + 2^k_{2r+1} where the signs alternate, there are an
 *          odd number of terms, and k_1 > k_2 > k_3 > ... > k_{2r+1} >= 0;
 *          sequence gives number of terms 2r+1.
 * https://oeis.org/A072219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   the number of terms in the unique alternating sum of powers of
 *            two * representation of n (OEIS A072219). The computation is
 *            recursive: * * Let p = smallest power of two ≥ n. * If n = p,
 *            the representation has 1 term. * Otherwise n = p – t and the
 *            remaining terms form an alternation with * an even...
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9323 (2026-11-01)
 * Helpers:   is_pow2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_pow2_idx.h"

/* Return the smallest power of two that is ≥ n. */
static inline idx_t next_power_of_two_idx(idx_t n)
{
    if (n == 0)
        return 1;
    if (is_pow2_idx(n))
        return n;
    /* Find the highest set bit position. */
    idx_t res = 1;
    while (res < n)
        res <<= 1;
    return res;
}

/* Forward declarations. */
static seq_t odd_count(idx_t n);   /* a(n) for odd-length representation */
static seq_t even_count(idx_t n);  /* number of terms in even-length representation */

/* Count terms in the unique odd-length representation of n. */
static seq_t odd_count(idx_t n)
{
    if (is_pow2_idx(n))
        return 1;

    idx_t p    = next_power_of_two_idx(n);
    idx_t t    = p - n;          /* remainder, 0 < t < n */
    seq_t e    = even_count(t);  /* number of even terms */
    return 1 + e;                /* leading positive term + rest */
}

/* Count terms in a representation with an even number of terms. */
static seq_t even_count(idx_t n)
{
    if (is_pow2_idx(n))
        return 2;                /* 2^(k+1) - 2^k */

    idx_t p    = next_power_of_two_idx(n);
    idx_t t    = p - n;          /* remainder, 0 < t < n */
    seq_t f    = odd_count(t);   /* odd-length representation of t */
    return 1 + f;                /* leading positive term + rest */
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, return 0 for n = 0. */
    if (n == 0)
        return 0;
    return odd_count(n);
}
