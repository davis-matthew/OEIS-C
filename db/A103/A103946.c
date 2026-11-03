/*
 * A103946  Indices of icosahedral numbers (A006564) which are semiprimes.
 * https://oeis.org/A103946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 350, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9060 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103946 – indices k such that the icosahedral number A006564(k) is a semiprime.
 *
 * The icosahedral number is M(k) = k * (5*k^2 - 5*k + 2) / 2.
 * The OEIS definition is Bigomega(M(k)) = 2, i.e. M(k) has exactly two prime
 * factors counted with multiplicity.  For any k, this is equivalent to
 * k being prime and (5*k^2 - 5*k + 2)/2 being prime, because if k were
 * composite M(k) would have at least three prime factors.
 *
 * Algorithm: iterate over integers k starting at 2, test k for primality,
 * then compute q = (5*k^2 - 5*k + 2)/2 and test q for primality.
 * When both are prime the k is recorded; the n‑th such k is returned.
 *
 * Complexity: roughly O(n log n) primality tests; well within limits for
 * n ≈ 10 000.  No memoisation is required, as calls may arrive in any order.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification. */
    idx_t count = 0;
    uint64_t k = 2;  /* start checking from the first integer > 1 */

    while (true) {
        /* Quick discard: if k is not prime, skip. */
        if (is_prime(k)) {
            /* Compute q = (5*k^2 - 5*k + 2) / 2 using 128‑bit to avoid overflow. */
            unsigned __int128 kk = (unsigned __int128)k;
            unsigned __int128 num = 5*kk*kk - 5*kk + 2;  /* numerator */
            unsigned __int128 q128 = num / 2;

            /* q must fit in 64‑bit for the available primality test. */
            if (q128 <= (unsigned __int128)UINT64_MAX) {
                uint64_t q = (uint64_t)q128;
                if (is_prime(q)) {
                    /* k is a valid term of the sequence. */
                    ++count;
                    if (count == n)
                        return (seq_t)k;
                }
            }
        }
        /* Move to the next integer.  k will overflow only after an astronomical
         * number of iterations, which will not happen for n ≈ 10 000. */
        ++k;
    }

    /* In practice this point is never reached.  Return 0 as a safe fallback. */
    return (seq_t)0;
}
