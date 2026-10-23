/*
 * A161117  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 14.
 * https://oeis.org/A161117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 17, u128 n <= 659,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161117
   a(n) = 16383 * ∏_{p^e‖n} ((p^13-1)/(p-1)) * p^{12(e-1)}   (b = 14)
   The product is empty for n = 1, giving a(1) = 16383.
*/

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Construct a power of a seq_t base with small integer exponent.
   Uses exponentiation by squaring.  Works for any seq_t that
   supports +, *, /, and casting from integer types.
------------------------------------------------------------------ */
static inline seq_t pow_uint_seq(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------
   Simple trial‑division factorisation for 64‑bit integers.
   Fills arrays `primes` (prime) and `exps` (exponent) with at most
   64 entries, and writes the number of factors into *cnt.
------------------------------------------------------------------ */
static void factor_uint64(uint64_t n, uint64_t *primes,
                          uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    uint64_t temp = n;

    if (temp < 2) {
        *cnt = 0;
        return;
    }

    /* factor 2 */
    if (temp % 2 == 0) {
        uint64_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        primes[i] = 2;
        exps[i]   = e;
        ++i;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[i] = p;
            exps[i]   = e;
            ++i;
        }
    }

    /* remaining prime */
    if (temp > 1) {
        primes[i] = temp;
        exps[i]   = 1;
        ++i;
    }

    *cnt = i;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1 */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* (p^13-1)/(p-1)  – exact division */
        seq_t p13       = pow_uint_seq((seq_t)p, 13);
        seq_t divisor   = (seq_t)(p - 1);
        seq_t term      = (p13 - 1) / divisor;
        result *= term;

        /* p^{12(e-1)}  – contributes only for e ≥ 2 */
        if (e > 1) {
            unsigned int exp = (unsigned int)(12 * (e - 1));
            seq_t p12exp = pow_uint_seq((seq_t)p, exp);
            result *= p12exp;
        }
    }

    /* multiply by 16383 = 2^14 - 1 */
    result *= (seq_t)16383;

    return result;
}
