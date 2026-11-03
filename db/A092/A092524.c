/*
 * A092524  Binary representation of n interpreted in base p, where p is the
 *          smallest prime factor of n: p = A020639(n).
 * https://oeis.org/A092524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A092524
 * Formula: a(1) = 1.
 * For n > 1, let p be the smallest prime factor of n
 * (p = 2 when n is even).
 * If n is even, a(n) = n.
 * If n is odd, a(n) = Σ_{i≥0} b_i · p^i,
 *   where n = Σ b_i·2^i (binary expansion of n, b_i ∈ {0,1}).
 */

#include <stdint.h>

/* return the smallest prime factor of n (n ≥ 2);  
 * if n is prime, returns n itself. */
static uint64_t _spf_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;          /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;              /* a(1) is defined explicitly */

    /* Even numbers: shortcut */
    if ((n & 1ULL) == 0) return (seq_t)n;

    /* n is odd.  Find its smallest prime factor. */
    uint64_t p = _spf_uint64((uint64_t)n);

    /* Interpret binary representation of n in base p. */
    seq_t result = 0;
    seq_t power  = 1;                         /* p^0 */
    idx_t temp   = n;

    while (temp) {
        if (temp & 1ULL)                      /* current bit is 1 */
            result += power;
        temp >>= 1;
        if (!temp) break;                     /* skip multiplication after last bit */
        power *= (seq_t)p;                    /* advance to next power of p */
    }

    return result;
}
