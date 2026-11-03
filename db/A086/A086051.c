/*
 * A086051  Nontrivial numbers which are prime, contain no zero digits and
 *          yield another prime when their digits are sorted in ascending
 *          order.
 * https://oeis.org/A086051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 927 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nontrivial primes with no zero digits, whose
   digits sorted in ascending order form another prime.
   The search is performed by brute force; candidates are memoised
   so repeated calls are O(1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
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

/* ------------------------------------------------------------------ */
/* Static storage for memoisation                                         */
/* ------------------------------------------------------------------ */
static seq_t     *seq_cache          = NULL;   /* array of sequence elements */
static size_t    cache_len           = 0;      /* number of elements stored */
static size_t    cache_cap           = 0;      /* allocated capacity */
static seq_t     next_candidate      = (seq_t)2;/* value to test next */

/* Utility: enlarge cache array when needed */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp) abort();          /* out of memory – terminate */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Checks whether a prime satisfies the OEIS condition. */
static bool qualifies_prime(seq_t p)
{
    /* fetch digits and length (digits are stored in reverse order) */
    uint8_t digits[64];
    size_t  len;
    get_digits(p, digits, &len);

    if (len <= 1) return false; /* single‑digit primes are excluded */

    /* count digits, detect zero digits */
    unsigned char cnt[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 0) return false; /* zero digit disqualifies */
        cnt[digits[i]]++;
    }

    /* build the ascending‑sorted number */
    seq_t asc = (seq_t)0;
    for (int d = 1; d <= 9; ++d)
        for (int c = 0; c < cnt[d]; ++c)
            asc = asc * (seq_t)10 + (seq_t)d;

    /* reject trivial case: already in ascending order */
    if (asc == p) return false;

    /* the ascending‑sorted number must also be prime */
    return is_prime_big(asc);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return a safe value if called with 0 */
        return (seq_t)0;

    /* Expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        grow_cache(cache_len + 1);

        /* Find the next prime */
        while (!is_prime_big(next_candidate))
            ++next_candidate;

        seq_t candidate = next_candidate;
        ++next_candidate;   /* advance for the next loop */

        /* If it qualifies, store it */
        if (qualifies_prime(candidate))
            seq_cache[cache_len++] = candidate;
    }

    return seq_cache[n - 1];
}
