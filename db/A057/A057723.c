/*
 * A057723  Sum of positive divisors of n that are divisible by every prime
 *          that divides n.
 * https://oeis.org/A057723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p + p² + … + p^e)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th term of OEIS sequence A057723.
    It uses trial‑division factorisation which is fast enough for
    all arguments that appear in the test harness (n ≤ ~10⁵).   */

#include <stdint.h>
#include <stddef.h>

/* Helper: factorises a 64‑bit integer.  The caller supplies buffers
   large enough to hold all prime factors (at most 64).          */
static void factor_uint64(uint64_t n, uint64_t primes[64],
                          uint64_t exps[64], size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;

    if (m == 0 || m == 1) {
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
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }

    if (m > 1) {               /* remaining prime factor */
        primes[c] = m;
        exps[c]   = 1;
        ++c;
    }

    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 in OEIS offset */
    if (n == 1) return (seq_t)1;  /* 1 has no prime factors: product=1 */

    /* Use 64‑bit variables for intermediate factorisation. */
    uint64_t nn = (uint64_t)n;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count;

    factor_uint64(nn, primes, exps, &count);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < count; ++i) {
        seq_t p   = (seq_t)primes[i];
        size_t e  = (size_t)exps[i];

        seq_t term = (seq_t)0;
        seq_t pow  = p;              /* p¹ */

        for (size_t k = 1; k <= e; ++k) {
            term += pow;
            if (k < e) pow *= p;     /* prepare next power */
        }

        result *= term;
    }

    return result;
}
