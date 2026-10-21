/*
 * A061782  a(n) = smallest positive number m such that m*n is a triangular
 *          number.
 * https://oeis.org/A061782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest positive integer m such that m*n is a triangular
 *            number. For a given n, find the smallest k>0 with k(k+1)
 *            divisible by 2*n. Then a(n) = k(k+1)/(2*n). k is the least
 *            positive solution of the congruences k ≡ 0 or –1 (mod p^e) for
 *            every prime power p^e dividing 2*n. All such k values...
 * Range:     u8 n <= 255, u16 n <= 7847, u32 n <= 7847, u64 n <= 7847,
 *            u128 n <= 7847, big any n
 * Helpers:   egcd_inverse_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/egcd_inverse_uint64.h"

static uint64_t pow_uint64(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = (unsigned __int128)result * base;
        exp >>= 1;
        if (exp) base = (unsigned __int128)base * base;
    }
    return result;
}

static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    while ((n & 1) == 0) {
        if (c == 0 || pr[c-1] != 2) {
            pr[c] = 2; ex[c] = 0; c++;
        }
        ex[c-1]++; n >>= 1;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            if (c == 0 || pr[c-1] != p) {
                pr[c] = p; ex[c] = 0; c++;
            }
            ex[c-1]++; n /= p;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; c++; }
    *cnt = c;
}

static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Core routine that works with 64‑bit integers. */
static uint64_t compute_a_uint64(uint64_t n)
{
    /* factor 2*n */
    uint64_t two_n = n << 1;   /* safe for n <= 2^63-1 */

    uint64_t primes[64], exps[64];
    size_t pc;
    factor_uint64(n, primes, exps, &pc);

    /* adjust for the extra factor of 2 in 2*n */
    bool seen2 = false;
    for (size_t i = 0; i < pc; ++i) {
        if (primes[i] == 2) { exps[i]++; seen2 = true; break; }
    }
    if (!seen2) { primes[pc] = 2; exps[pc] = 1; pc++; }

    /* compute individual prime powers */
    uint64_t moduli[64];
    for (size_t i = 0; i < pc; ++i) moduli[i] = pow_uint64(primes[i], exps[i]);

    uint64_t best_k = UINT64_MAX;
    uint64_t masks = (uint64_t)1 << pc;

    for (uint64_t mask = 0; mask < masks; ++mask) {
        uint64_t X = 0, M = 1;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t a   = (mask & (1ULL << i)) ? (moduli[i] - 1) : 0;
            uint64_t m_i = moduli[i];

            uint64_t inv          = egcd_inverse_uint64(M % m_i, m_i);
            uint64_t Xmod          = X % m_i;
            uint64_t delta         = (a + m_i - Xmod) % m_i;
            uint64_t t             = mod_mul_uint64(delta, inv, m_i);
            unsigned __int128 newX = (unsigned __int128)X + (unsigned __int128)M * t;
            uint64_t newM          = M * m_i;
            X = (uint64_t)(newX % newM);
            M = newM;
        }
        uint64_t k = X;
        if (k == 0) k = M;            /* make k positive */
        if (k < best_k) best_k = k;
    }

    /* compute a(n) = k(k+1)/(2*n) */
    unsigned __int128 num = (unsigned __int128)best_k * (best_k + 1);
    uint64_t den = (unsigned __int128)n * 2;
    return (uint64_t)(num / den);
}

/* Public function with the required signature */
seq_t a(idx_t n)
{
    return (seq_t)compute_a_uint64((uint64_t)n);
}
