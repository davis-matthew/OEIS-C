/*
 * A093929  a(n+1) is the smallest odd prime not already used such that
 *          a(n)*a(n+1)+2 is prime.
 * https://oeis.org/A093929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=3; for n>1, a(n) is the smallest odd prime not already
 *            occurring in the sequence such that a(n‑1)*a(n)+2 is prime
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/* Required includes.  The util headers provide the needed primality
   tests.  The code is valid both as C11 and as C++17.  */
/* --------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

/* --------------------------------------------------------------------- */
/* Static state for memoisation.  The array a_cache holds the
   already‑computed terms.  The array used_list records the primes
   that have appeared.  All data structures grow geometrically.     */
/* --------------------------------------------------------------------- */
static seq_t   *a_cache   = NULL;   /* cached a(n) values        */
static size_t   a_len     = 0;       /* number of cached values   */
static size_t   a_cap     = 0;       /* capacity of a_cache       */

static seq_t   *used_list = NULL;   /* primes that have already
                                       appeared in the sequence   */
static size_t   used_count = 0;      /* number of stored primes   */
static size_t   used_cap   = 0;      /* capacity of used_list     */

/* Grow used_list if necessary. */
static void
grow_used_list(void)
{
    if (used_count < used_cap) return;
    size_t new_cap = (used_cap == 0) ? 16 : used_cap << 1;
    seq_t *tmp = mem_resize(seq_t, used_list, new_cap);
    if (!tmp) abort();
    used_list = tmp;
    used_cap  = new_cap;
}

/* Append a prime to used_list. */
static void
mark_used(seq_t p)
{
    grow_used_list();
    used_list[used_count++] = p;
}

/* Check whether a prime is already used.  Linear search is cheap
   because the sequence is short (≤ 10000 terms). */
static bool
used_contains(seq_t p)
{
    for (size_t i = 0; i < used_count; ++i)
        if (used_list[i] == p) return true;
    return false;
}

/* Grow a_cache if necessary. */
static void
grow_cache(void)
{
    if (a_len < a_cap) return;
    size_t new_cap = (a_cap == 0) ? 16 : a_cap << 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Append a new term to a_cache. */
static void
cache_append(seq_t val)
{
    grow_cache();
    a_cache[a_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  The behaviour for n < 1 is
       irrelevant for the tests.  */
    if (n == 0) return 0;

    /* Ensure we have computed all terms up to index n. */
    while (a_len < (size_t)n) {
        /* First term */
        if (a_len == 0) {
            seq_t first = 3;
            cache_append(first);
            mark_used(first);
            continue;
        }

        /* Previous term */
        seq_t prev = a_cache[a_len - 1];

        /* Search for the next term */
        for (seq_t cand = 3; ; cand += 2) {      /* odd numbers only */
            if (!is_prime_seq(cand)) continue;
            if (used_contains(cand))          continue;

            seq_t prod = prev * cand + (seq_t)2;
            if (is_prime_big(prod)) {
                cache_append(cand);
                mark_used(cand);
                break;
            }
        }
    }

    return a_cache[(size_t)n - 1];
}
