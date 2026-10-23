/*
 * A033205  Primes of form x^2 + 5*y^2.
 * https://oeis.org/A033205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form x^2 + 5*y^2. A prime p > 5 is such a prime
 *            iff p ≡ 1 or 9 (mod 20). 5 itself is also included
 * Range:     u8 n <= 11, u16 n <= 1602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"

/*--- modular arithmetic for 64‑bit integers ---------------------------------*/

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/*--- Miller–Rabin primality test for 64‑bit integers ------------------------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* quick check against small primes */
    static const uint64_t small_primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* write n‑1 = d · 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic set of bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/*--- cache for the sequence ----------------------------------------------*/
static seq_t *seq_cache = NULL;
static size_t seq_cnt   = 0;
static size_t seq_cap   = 0;
static uint64_t next_candidate = 5;

static void add_value(seq_t v)
{
    if (seq_cap <= seq_cnt) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* allocation failure */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_cnt++] = v;
}

/*--- sequence function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, but safe */

    /* compute until we have enough terms */
    while (seq_cnt < n) {
        uint64_t p = next_candidate;

        /* check residue condition: 5 or ≡ 1,9 (mod 20) */
        if (p == 5 || (p % 20 == 1) || (p % 20 == 9)) {
            if (is_prime_uint64(p)) {
                add_value((seq_t)p);
            }
        }

        /* advance to the next odd candidate */
        next_candidate += 2;
    }

    return seq_cache[n - 1];
}
