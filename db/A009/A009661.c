/*
 * A009661  Smallest number m such that m^m+1 is divisible by n.
 * https://oeis.org/A009661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */

/* Computes the smallest m such that m^m + 1 is divisible by n.
 * The algorithm searches m from 0 up to the bounds given in the OEIS
 * description: for even n the bound is n‑1, for odd n it is 2n‑1.
 * Modulo exponentiation is performed by the helper mod_pow_local. */
static seq_t mod_pow_local(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;        /* mod ≥ 1 for n ≥ 1 */
    seq_t cur    = base % mod;

    while (exp > 0) {
        if (exp & 1)
            result = (result * cur) % mod;
        cur = (cur * cur) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base cases: for n=1 or 2 the smallest m is 0 (0^0+1 is 2,
     * which is divisible by 1 and 2). */
    if (n <= 2)
        return 0;

    seq_t N       = (seq_t)n;      /* modulus n */
    seq_t target  = N - 1;         /* need m^m ≡ N-1 (mod N) */
    idx_t bound   = (n & 1ULL) ? (2 * n - 1) : (n - 1);

    for (idx_t m = 0; m <= bound; ++m) {
        seq_t val = mod_pow_local((seq_t)m, (uint64_t)m, N);
        if (val == target)
            return (seq_t)m;
    }

    /* The OEIS guarantees a solution within the bound; this line is
     * theoretically unreachable.  Returning the bound as a fallback
     * keeps the function well‑defined. */
    return (seq_t)bound;
}
