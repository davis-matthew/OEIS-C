/*
 * A160500  Prime p1 of consecutive primes p1, p2, where p2-p1=10, and p1, p2
 *          are in different centuries.
 * https://oeis.org/A160500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160500: primes p1 such that the next prime p2 satisfies
   p2 - p1 = 10 and floor(p1/100) != floor(p2/100).  The function
   computes the nth term (offset 1).  It uses a deterministic
   Miller–Rabin test for 64‑bit integers and incremental sieving,
   independent of the concrete type chosen for seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1U) result = mod_mul(result, b, mod);
        b = mod_mul(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Small primes check */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    size_t i;
    for (i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller–Rabin with bases valid for 64‑bit integers */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t bi = 0; bi < sizeof(bases) / sizeof(bases[0]); ++bi)
    {
        uint64_t a = bases[bi];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1ULL; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Data structures and helper functions ---------- */
static idx_t   seq_len  = 0;   /* number of terms stored (idx_t) */
static size_t  seq_cap  = 0;   /* allocated capacity (size_t) */
static seq_t  *seq_vals = NULL;/* array of sequence terms */

static uint64_t last_prime      = 0; /* last prime seen while extending */
static idx_t    current_limit   = 0; /* highest number currently processed */

/* Add a term to the sequence, reallocating if necessary */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2ULL : 128ULL;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Extend the search to at least 'limit', generating new terms as primes are found */
static void extend_primes_to(idx_t limit)
{
    if (limit <= current_limit) return;

    for (idx_t cand = current_limit + 1; cand <= limit; ++cand)
    {
        if (cand == 2)
        {
            if (last_prime != 0 && (cand - last_prime == 10ULL))
            {
                uint64_t lp_cent = last_prime / 100ULL;
                uint64_t lc_cent = cand / 100ULL;
                if (lp_cent != lc_cent) add_term((seq_t)last_prime);
            }
            last_prime = cand;
            continue;
        }
        if ((cand & 1ULL) == 0U) continue; /* skip even numbers >2 */

        if (is_prime_uint64((uint64_t)cand))
        {
            uint64_t cand64 = (uint64_t)cand;
            if (last_prime != 0 && (cand64 - last_prime == 10ULL))
            {
                uint64_t lp_cent = last_prime / 100ULL;
                uint64_t lc_cent = cand64 / 100ULL;
                if (lp_cent != lc_cent) add_term((seq_t)last_prime);
            }
            last_prime = cand64;
        }
    }
    current_limit = limit;
}

/* Ensure at least 'n' terms are computed */
static void ensure_n(idx_t n)
{
    if (seq_len >= n) return;

    idx_t step = 2000000ULL; /* initial step size (2 million) */
    while (seq_len < n)
    {
        extend_primes_to(current_limit + step);
        if (step < 100000000ULL) step <<= 1ULL; /* grow step for larger ranges if needed */
    }
}

/* ---------- Public entry point ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behaviour below offset is unspecified */
    ensure_n(n);
    return seq_vals[n - 1];
}
