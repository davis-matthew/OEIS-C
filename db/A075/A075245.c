/*
 * A075245  x-value of the solution (x,y,z) to 4/n = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z and having the largest z-value. The y and
 *          z components are in A075246 and A075247.
 * https://oeis.org/A075245
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  hard, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 1056, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A075245: x-value of the solution (x,y,z) to 
 * 4/n = 1/x + 1/y + 1/z with 0 < x < y < z and having the
 * largest z-value.
 *
 * The algorithm enumerates all feasible x-values, and for each x
 * it computes the largest possible z by exploring all divisors
 * of (n*x)^2 that satisfy the parametrisation
 *   A = 4*x - n,  B = n*x,
 *   u = A*y - B, v = A*z - B,  u*v = B^2.
 * The solution with the maximum z is selected, and its x is
 * returned.
 *
 * Complexity: O(n) per call for n up to ~10,000 (≈ millions of
 * simple integer operations) – well within the 2‑second limit.
 * No external libraries are required beyond standard headers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 * Prime sieve – needed for factorisation of B = n*x.
 * --------------------------------------------------------------- */
#define PRIME_LIMIT 100000U
static uint32_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;         /* already initialised */

    static bool is_comp[PRIME_LIMIT + 1];
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
            {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
}

/* ------------------------------------------------------------------
 * For a given n and x, find the largest possible z.
 * --------------------------------------------------------------- */
static bool max_z_for_x(idx_t n, idx_t x, seq_t *out_z)
{
    if (x <= 0) return false;
    seq_t A = 4 * (seq_t)x - (seq_t)n;   /* A > 0 for all valid x */
    if (A <= 0) return false;

    seq_t B = (seq_t)n * (seq_t)x;       /* fits in seq_t (≤ 75 million) */

    /* ------------------------------------------------------------------
     * Factorise B.  Store prime factors and their exponents.
     * ------------------------------------------------------------------ */
    uint64_t pf[32];
    uint8_t exp[32];
    size_t k = 0;
    uint64_t temp = (uint64_t)B;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0)
        {
            uint8_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            pf[k]   = p;
            exp[k]  = e;
            ++k;
        }
    }
    if (temp > 1)
    {
        pf[k]  = temp;
        exp[k] = 1;
        ++k;
    }

    /* ------------------------------------------------------------------
     * Recursively enumerate all divisors of B^2.
     * For each divisor u, compute v = B^2 / u and test the
     * divisibility conditions:
     *      (u + B) % A == 0  and  (v + B) % A == 0
     * If satisfied, compute y,z and keep the maximum z.
     * ------------------------------------------------------------------ */
    const uint64_t B64     = (uint64_t)B;
    const uint64_t B2      = (uint64_t)B * (uint64_t)B;   /* fits in 64‑bit */
    seq_t best_z      = 0;
    bool found_any    = false;

    /* helper lambda for recursion – uses a simple stack */
    /* The recursion depth (k) is small (≤~10). */
    /* We store the current product in __int128 to avoid overflow. */
    struct RecParams
    {
        size_t pos;
        __int128 cur;
    };

    /* stack for manual recursion (max depth k+1).  k ≤ 32. */
    struct RecParams stack[36];
    size_t sp = 0;

    stack[sp++] = (struct RecParams){0, 1};

    while (sp > 0)
    {
        struct RecParams node = stack[--sp];
        if (node.pos == k)
        {
            uint64_t u = (uint64_t)node.cur;
            if (u == 0 || u > B2) continue;   /* safety */
            uint64_t v = B2 / u;
            if (((u + B64) % A) != 0) continue;
            if (((v + B64) % A) != 0) continue;

            seq_t y = (seq_t)((u + B64) / A);
            if (y <= (seq_t)x) continue;
            seq_t z = (seq_t)((v + B64) / A);
            if (z <= y) continue;

            if (z > best_z)
            {
                best_z = z;
                found_any = true;
            }
            continue;
        }

        /* branch: choose exponent for current prime (0 … 2*exp[pos]) */
        uint64_t prime = pf[node.pos];
        uint8_t maxe = 2 * exp[node.pos];

        /* Precompute prime powers iteratively to avoid repeated multiplication */
        uint64_t pow_val = 1;
        for (uint8_t e = 0; e <= maxe; ++e)
        {
            /* push child onto stack */
            stack[sp++] = (struct RecParams){node.pos + 1, node.cur * pow_val};

            /* prepare next power */
            pow_val = pow_val * prime;
        }
    }

    if (found_any)
    {
        *out_z = best_z;
        return true;
    }
    return false;
}

/* ------------------------------------------------------------------
 * Main sequence function.
 * --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* offset is 3 */

    init_primes();                        /* ensure primes are ready */

    /* Search all feasible x:  n/4 < x <= 3n/4   (strict inequalities give
     * the same range in integer arithmetic) */
    idx_t min_x = n / 4 + 1;               /* floor(n/4)+1 */
    idx_t max_x = n * 3 / 4;               /* floor(3n/4) */

    seq_t best_z = 0;
    idx_t  best_x = 0;

    for (idx_t x = min_x; x <= max_x; ++x)
    {
        seq_t z;
        if (max_z_for_x(n, x, &z))
        {
            if (z > best_z)
            {
                best_z = z;
                best_x = x;
            }
        }
    }

    /* As a safety fallback (should never be needed), if no solution was
     * found in the expected range, try a broader search. */
    if (best_x == 0)
    {
        for (idx_t x = 1; x <= n; ++x)
        {
            seq_t z;
            if (max_z_for_x(n, x, &z))
            {
                if (z > best_z)
                {
                    best_z = z;
                    best_x = x;
                }
            }
        }
    }

    return (seq_t)best_x;
}
