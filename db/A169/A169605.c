/*
 * A169605  Numbers x of the form x = 2*y - 3 = 3*z - 2 where y and z are
 *          primes.
 * https://oeis.org/A169605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169605: numbers x such that x = 2*y - 3 = 3*z - 2 with y and z primes
   The routine scans primes y≡2 (mod 3); for each such prime it tests whether
   z = (2*y-1)/3 is prime, and stores x = 2*y-3.  The sequence values are
   cached so that arbitrary order of calls is supported.  Build limits
   are generous enough to generate >10 000 terms.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- configuration ---------- */
#define PRIMES_UPTO 15000000ULL           /* enough to cover n≈10 000 */

/* ---------- static storage ---------- */
static idx_t   *prime_arr = NULL;         /* list of primes up to PRIMES_UPTO */
static size_t   prime_cnt  = 0;           /* number of primes in prime_arr */

static seq_t  *cache = NULL;             /* computed sequence values */
static size_t  cache_len   = 0;           /* number of values stored */
static size_t  cache_cap   = 0;           /* current capacity of cache */

static size_t  last_prime_idx = 0;        /* next index in prime_arr to inspect */

/* ---------- helpers ---------- */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();  /* defensive: abort on OOM */
    cache = tmp;
    cache_cap = new_cap;
}

static void ensure_primes(void)
{
    if (!prime_arr) {
        /* build_primes_upto expects a pointer to idx_t* */
        build_primes_upto(PRIMES_UPTO, &prime_arr, &prime_cnt);
    }
}

/* Generate terms until the cache contains at least n entries.  */
static void generate_terms_up_to(idx_t n)
{
    /* n is unsigned, but cache_len is size_t.  use size_t comparison. */
    while (cache_len < (size_t)n) {
        /* search for the next suitable prime y */
        while (true) {
            if (last_prime_idx >= prime_cnt) {
                /* The prime limit was too low; abort – this should not happen
                   for n ≤ 10 000 with PRIMES_UPTO 15 million.  */
                abort();
            }
            idx_t y = prime_arr[last_prime_idx++];
            /* y must be congruent to 2 mod 3 to make z integer */
            if ((y % 3) != 2) continue;

            /* z = (2*y-1)/3  – integer because we checked the modulus   */
            seq_t z = ((seq_t)2 * (seq_t)y - 1) / 3;
            if (!is_prime_big(z)) continue;     /* z not prime */

            /* x = 2*y-3  – the sequence value                      */
            seq_t x = (seq_t)2 * (seq_t)y - 3;

            /* store in cache */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = x;

            break; /* move to next term                 */
        }
    }
}

/* ---------- OEIS entry ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence (offset = 1) */

    ensure_primes();
    generate_terms_up_to(n);

    return cache[(size_t)n - 1];
}
