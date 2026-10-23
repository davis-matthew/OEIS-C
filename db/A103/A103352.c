/*
 * A103352  Denominators of sum_{k=1..n} 1/k^9 = Zeta(9,n).
 * https://oeis.org/A103352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = L / gcd(P, L) * where L = Π_{p ≤ n} p^(9 * ⌊log_p(n)⌋) *
 *            P = Σ_{k=1..n} L / k^9 (the division is exact) * * The function
 *            returns the denominator of the reduced sum Σ_{k=1}^{n} 1/k^9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: integer power – pure multiplication loop (exponent ≤ 9*log₂n) */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Helper: Euclidean gcd for seq_t (works for all allowed seq_t types) */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 0 and 1 are trivial cases – they are not in the OEIS offset,
       but we handle them robustly here. */
    if (n <= 1)
        return (seq_t)1;

    /* ---------- 1. Sieve of Eratosthenes up to n (boolean array) ---------- */
    size_t len = (size_t)n;
    bool *is_prime = (bool *)malloc((len + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;          /* allocation failure – not expected in tests */

    for (size_t i = 0; i <= len; ++i)
        is_prime[i] = true;
    if (len >= 0) is_prime[0] = false;
    if (len >= 1) is_prime[1] = false;

    for (size_t i = 2; (size_t)i * i <= len; ++i)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i; j <= len; j += i)
                is_prime[j] = false;
        }
    }

    /* ---------- 2. Compute L = Π_{p ≤ n} p^(9*⌊log_p(n)⌋) ----------------- */
    seq_t L = (seq_t)1;
    for (size_t p = 2; p <= len; ++p)
    {
        if (!is_prime[p])
            continue;

        /* compute floor(log_p(n)) */
        uint64_t power_p = (uint64_t)p;
        uint32_t exp = 0;
        while (power_p <= (uint64_t)n)
        {
            ++exp;
            power_p *= p;
        }

        idx_t total_exp = (idx_t)(9ULL * exp);
        seq_t p_pow = seq_pow((seq_t)p, total_exp);
        /* Since primes are disjoint, no gcd is needed; just multiply. */
        L *= p_pow;
    }

    free(is_prime);

    /* ---------- 3. Compute P = Σ_{k=1..n} L / k^9 ------------------------ */
    seq_t P = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k_pow9 = seq_pow((seq_t)k, (idx_t)9);   /* k^9 */
        seq_t term   = L / k_pow9;                   /* exact division */
        P += term;
    }

    /* ---------- 4. Reduce: denominator = L / gcd(P, L) ------------------- */
    seq_t g   = gcd_seq(P, L);
    seq_t denom = L / g;

    return denom;
}
