/*
 * A216883  Primes p such that x^31 = 2 has a solution mod p.
 * https://oeis.org/A216883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216883 – primes p for which x^31 ≡ 2 (mod p) has a solution.
    This implementation generates primes incrementally and keeps a
    list of those that satisfy the condition.  No external utilities
    are required.  The code works for any seq_t (uint8_t … __int128)
    and idx_t = uint64_t.  All arithmetic that may overflow the
    native word size is done via unsigned __int128.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* forward declarations of helper routines */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr);
static bool qualifies(uint64_t p);
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod);

/* ------------------------------------------------------------------ */
/* static storage for primes and qualifying primes                   */
static uint64_t *all_primes      = NULL;   /* all primes found so far   */
static size_t    all_cnt   = 0;            /* number of primes in all_primes */
static size_t    all_cap   = 0;            /* allocated space in all_primes */

static uint64_t *qual_primes     = NULL;   /* primes that satisfy the test */
static size_t    qual_cnt   = 0;            /* number of qualifying primes  */
static size_t    qual_cap   = 0;            /* allocated space in qual_primes */

static uint64_t next_candidate = 2;        /* candidate for next prime */

/* ------------------------------------------------------------------ */
/* ensure that at least n qualifying primes are stored                */
static void ensure_quali(idx_t n)
{
    if (qual_cnt >= n)
        return;

    /* grow the prime lists if necessary */
    if (all_cap == 0) {
        all_cap = 64;
        all_primes = (uint64_t *)malloc(all_cap * sizeof(uint64_t));
    }
    if (qual_cap == 0) {
        qual_cap = 64;
        qual_primes = (uint64_t *)malloc(qual_cap * sizeof(uint64_t));
    }

    /* generate primes until we have enough qualifying ones */
    while (qual_cnt < n) {
        /* test next_candidate for primality */
        bool is_prime = true;
        /* check divisibility using previously found primes */
        for (size_t i = 0; i < all_cnt; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* store the new prime */
            if (all_cnt == all_cap) {
                all_cap *= 2;
                all_primes = (uint64_t *)realloc(all_primes,
                    all_cap * sizeof(uint64_t));
            }
            all_primes[all_cnt++] = next_candidate;

            if (qualifies(next_candidate)) {
                if (qual_cnt == qual_cap) {
                    qual_cap *= 2;
                    qual_primes = (uint64_t *)realloc(qual_primes,
                        qual_cap * sizeof(uint64_t));
                }
                qual_primes[qual_cnt++] = next_candidate;
            }
        }

        /* advance to the next odd number after 2 */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* ------------------------------------------------------------------ */
/* helper: test whether a prime p satisfies the condition             */
static bool qualifies(uint64_t p)
{
    /* If 31 does not divide p-1, any a has a 31st root in F_p.   */
    if ((p - 1) % 31 != 0)
        return true;

    /* Here p ≡ 1 (mod 31); need 2^((p-1)/31) ≡ 1 (mod p).      */
    uint64_t exp = (p - 1) / 31;
    return mod_pow_uint64(2, exp, p) == 1;
}

/* ------------------------------------------------------------------ */
/* modular exponentiation: (base^exp) mod mod, all 64‑bit values       */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* public sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for n<1, but avoid UB in caller */
        return (seq_t)0;
    ensure_quali(n);
    return (seq_t)qual_primes[n - 1];
}
