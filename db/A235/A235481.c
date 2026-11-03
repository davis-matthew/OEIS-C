/*
 * A235481  Primes whose base-4 representation is also the base-9
 *          representation of a prime.
 * https://oeis.org/A235481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of primes whose base‑4 representation is also the *
 *            base‑9 representation of a prime. a(1)=2, a(2)=3, a(3)=29, ... *
 *            This implementation keeps a static cache of discovered terms.
 *            For * each new candidate prime p it computes the integer q
 *            obtained by * interpreting the base‑4 digits of p...
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 6496, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9747 (2026-11-03)
 * Helpers:   bit_length, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/bit_length.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Convert a 64‑bit integer p to the integer obtained by
 * interpreting its base‑4 digits as a base‑9 number.
 * Digits are collected least‑significant first, then reversed
 * when building q so that the most‑significant digit is added
 * last, giving the correct value. */
static seq_t base4_to_base9(uint64_t p)
{
    uint8_t digits[64];
    size_t len = 0;

    if (p == 0)
        return 0;

    while (p > 0) {
        digits[len++] = p & 3;   /* p % 4 */
        p >>= 2;                /* p /= 4 */
    }

    seq_t q = 0;
    /* Build q from most significant digit to least */
    for (size_t i = len; i-- > 0; ) {
        q = q * 9 + digits[i];
    }
    return q;
}

/* Cache storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Extend the cache until it contains at least n elements. */
static void generate_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    static uint64_t last_checked = 2;  /* next integer to examine */

    for (;;) {
        uint64_t p = last_checked++;

        /* Skip non‑primes quickly */
        if (!is_prime((idx_t)p))
            continue;

        seq_t q = base4_to_base9(p);

        bool qprime;
        if (bit_length(q) <= 64)
            qprime = is_prime((idx_t)q);
        else
            qprime = is_prime_big(q);

        if (qprime)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);

        if (cache_len >= (size_t)n)
            break;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n<1, harmless return */
        return 0;

    generate_terms(n);
    return cache[n - 1];
}
