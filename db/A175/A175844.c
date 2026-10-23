/*
 * A175844  Parse the base-2 expansion of 1/n using the Ziv-Lempel encoding as
 *          described in A106182; sequence gives the eventual period of the
 *          differences of the sequence of lengths of the successive phrases.
 * https://oeis.org/A175844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175844: a(n) = (ord(2, odd(n)))^2, where
 * odd(n) is n with all factors of 2 removed and
 * ord(2, m) is the multiplicative order of 2 modulo m. */

#include <stdint.h>
#include <stddef.h>

/* -----------------------------------------------------------------------
 * Utility: factorise a uint64_t by trial division.
 * The factorisations are stored in the buffers `pr` and `ex` and the
 * number of distinct prime factors is written to `cnt`.  The caller
 * must provide buffers large enough (64 in this implementation
 * suffices for n <= 10000).
 * ----------------------------------------------------------------------- */
static void factor_uint64(uint64_t m, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (m < 2) {
        *cnt = 0;
        return;
    }
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (m > 1) {
        pr[c] = m;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* -----------------------------------------------------------------------
 * Utility: compute 2^exp (mod mod).  All intermediate arithmetic uses
 * 64‑bit unsigned integers which are safe because mod <= 10000.
 * ----------------------------------------------------------------------- */
static uint64_t pow2_mod(uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = 2 % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* -----------------------------------------------------------------------
 * Compute a(n) for the OEIS sequence A175844.
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n = 0, defensive */

    /* Extract the odd part of n. */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL)
        m >>= 1U;

    /* For odd part 1 the period is 1. */
    if (m == 1ULL)
        return (seq_t)1;

    /* Compute Euler's totient φ(m). */
    uint64_t pr[64], ex[64];
    size_t cnt, i;
    factor_uint64(m, pr, ex, &cnt);

    uint64_t phi = m;
    for (i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        phi = phi / p * (p - 1ULL);
    }

    /* Compute the multiplicative order of 2 modulo m. */
    uint64_t ord = phi;
    factor_uint64(phi, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        while (ord % p == 0ULL) {
            uint64_t trial = ord / p;
            if (pow2_mod(trial, m) == 1ULL)
                ord = trial;
            else
                break;
        }
    }

    seq_t period = (seq_t)ord;
    return period * period;
}
