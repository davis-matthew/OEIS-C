/*
 * A038951  Primes p such that 69 is a square mod p.
 * https://oeis.org/A038951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes p such that 69 is a square modulo p.
 * The first few terms are: 3, 5, 11, 13, 17, 23, 31, 53, 73, 83, …
 *
 * This implementation keeps a growing list of primes and
 * a growing list of those primes for which 69 is a quadratic residue.
 * It reuses the powmod64 utility from the utils folder.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/powmod64.h"

static uint64_t   *prime_list   = NULL; /* primes seen so far */
static size_t      prime_len    = 0;
static size_t      prime_cap    = 0;
static uint64_t   *res_list     = NULL; /* primes satisfying the residue condition */
static size_t      res_len      = 0;
static size_t      res_cap      = 0;
static uint64_t    candidate    = 3;     /* next odd candidate to test */
static bool        initialized  = false;

/* Ensure prime_list has room for at least new_cap elements. */
static inline void ensure_prime_cap(size_t new_cap)
{
    if (prime_cap >= new_cap) return;
    size_t new_size = prime_cap ? prime_cap * 2 : 8;
    if (new_size < new_cap) new_size = new_cap;
    uint64_t *tmp = (uint64_t*)realloc(prime_list, new_size * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_size;
}

/* Ensure res_list has room for at least new_cap elements. */
static inline void ensure_res_cap(size_t new_cap)
{
    if (res_cap >= new_cap) return;
    size_t new_size = res_cap ? res_cap * 2 : 8;
    if (new_size < new_cap) new_size = new_cap;
    uint64_t *tmp = (uint64_t*)realloc(res_list, new_size * sizeof(uint64_t));
    if (!tmp) abort();
    res_list = tmp;
    res_cap = new_size;
}

/* Test if p is prime using the primes already found. */
static bool is_prime_candidate(uint64_t p)
{
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t pp = prime_list[i];
        if (pp * pp > p) break;
        if (p % pp == 0) return false;
    }
    return true;
}

/* Legendre/Euler test for 69 mod p.
   Primes dividing 69 (3 and 23) are added automatically. */
static bool is_quadratic_residue(uint64_t p)
{
    /* 69 mod p == 0 means p divides 69 → trivial square (0). */
    if ((69ULL % p) == 0) return true;
    return powmod64(69, (p - 1) / 2, p) == 1;
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for offset */

    /* Initialise once. */
    if (!initialized) {
        initialized = true;
        ensure_prime_cap(8);
        prime_list[0] = 2;
        prime_len = 1;
        ensure_res_cap(8);
    }

    /* Produce results until we have at least n of them. */
    while (res_len < (size_t)n) {
        /* Find next prime. */
        while (true) {
            if (is_prime_candidate(candidate)) {
                /* Record new prime. */
                ensure_prime_cap(prime_len + 1);
                prime_list[prime_len++] = candidate;

                /* Check residue condition and store if true. */
                if (candidate > 2 && is_quadratic_residue(candidate)) {
                    ensure_res_cap(res_len + 1);
                    res_list[res_len++] = candidate;
                }
                break;
            }
            candidate += 2;          /* skip evens when trial division fails */
        }
        candidate += 2;              /* advance to next odd candidate */
    }

    return (seq_t)res_list[(size_t)n - 1];
}
