/*
 * A213931  Smallest number k such that the sum of divisors of k equals n
 *          times a nontrivial integer power.
 * https://oeis.org/A213931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 642, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A213931:
   a(n) = the smallest integer k such that sigma(k) = n * m^q
   with integers m >= 2 and q >= 2.  The computation is performed
   by a brute‑force search over k, using only integer arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Return the integer k-th root of x (k >= 2), or 0 if x is not an exact k-th power. */
static seq_t kth_root(seq_t x, idx_t k)
{
    if (k < 2) return 0;
    seq_t low  = 2;
    seq_t high = x;
    seq_t best = 0;               /* just to keep the compiler happy */
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        /* compute mid^k, stopping if it exceeds x */
        seq_t pow_mid = 1;
        for (idx_t i = 0; i < k; ++i) {
            pow_mid *= mid;
            if (pow_mid > x) break;
        }
        if (pow_mid == x) return mid;
        if (pow_mid < x) {
            low  = mid + 1;
            best = mid;
        } else {
            high = mid - 1;
        }
    }
    return 0;   /* not an exact k-th power */
}

/* Test whether v is a perfect power m^q with m >= 2 and q >= 2.
   Only integer arithmetic is used. */
static bool is_perfect_power(seq_t v)
{
    if (v < 4) return false;       /* smallest perfect power with m,q>=2 is 4=2^2 */
    /* We only need to test exponents up to 64; larger exponents would require a
       base of at most 2, which would have been caught with q=2. */
    for (idx_t exp = 2; exp <= 64; ++exp) {
        seq_t root = kth_root(v, exp);
        if (root >= 2) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t for modulus operations. */
    seq_t n_seq = (seq_t)n;
    idx_t k = 1;
    while (1) {
        seq_t sigma = sigma_of(k);               /* sigma(k) */
        if ((sigma % n_seq) == 0) {
            seq_t t = sigma / n_seq;             /* t must be m^q */
            if (t >= 4 && is_perfect_power(t))
                return (seq_t)k;
        }
        ++k;
    }
}
