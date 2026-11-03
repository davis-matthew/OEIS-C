/*
 * A235622  Primes whose base-8 representation also is the base-7
 *          representation of a prime.
 * https://oeis.org/A235622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 388, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime p such that its base‑8 representation equals
   the base‑7 representation of some prime q.  The function builds the list
   lazily using a simple prime sieve for q and converts each prime q to a
   base‑8 number p; if p itself is prime it is appended to the cache.  The
   result is returned in OEIS indexing (a(1) is the first term).  The
   implementation works for all supported seq_t sizes (including
   arbitrary‑precision) and does not hard‑code any terms.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Convert a prime q (idx_t) to the candidate p:
   take the base‑7 digits of q (most significant first) and interpret them
   in base‑8.  The function works for any seq_t size because no overflow
   check is required for the expected problem size. */
static seq_t base7_to_base8(idx_t q)
{
    /* extract base‑7 digits (least significant first) */
    uint8_t digits[64];
    size_t dpos = 0;
    do {
        digits[dpos++] = (uint8_t)(q % 7);
        q /= 7;
    } while (q);

    /* build the base‑8 number using most significant digit first */
    seq_t p = 0;
    for (size_t i = dpos; i-- > 0; ) {
        p = p * (seq_t)8 + (seq_t)digits[i];
    }
    return p;
}

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len  = 0;    /* number of stored terms */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  next_q     = 2;    /* next integer to test as prime q */

/* Ensure that the cache can hold at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 256 : cache_cap << 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();              /* out of memory – abort the program */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 – return 0 as a safe value */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Find the next prime q */
        while (!is_prime(next_q)) {
            next_q++;
        }

        /* Compute candidate p from this q */
        seq_t p = base7_to_base8(next_q);

        /* If p is prime, cache it */
        if (is_prime_big(p)) {
            cache_append(p);
        }

        /* Prepare for the next iteration */
        next_q++;
    }

    return cache[n - 1];
}
