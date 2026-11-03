/*
 * A136073  Primes of the form 9*p + 8 with p prime.
 * https://oeis.org/A136073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9831 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th prime of the form 9*p + 8, where p itself is prime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Cached sequence values. */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Last integer p that was examined. */
static uint64_t last_checked = 1; /* start just before the first prime */

/* Ensure the terms array has space for at least 'needed' elements. */
static void grow_terms(size_t needed)
{
    if (terms_len >= needed) return;
    if (terms_cap == 0) terms_cap = 4;
    while (terms_cap < needed) terms_cap <<= 1;
    terms = mem_resize(seq_t, terms, terms_cap);
}

/* Generate terms up to index n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (terms_len >= (size_t)n) return; /* already have enough */

    uint64_t cand;
    for (cand = last_checked + 1; terms_len < (size_t)n; ++cand)
    {
        /* Skip even numbers > 2. */
        if (cand > 2 && !(cand & 1U)) continue;

        if (!is_prime(cand)) continue;        /* composite */

        /* p = cand is prime. */
        seq_t q = (seq_t)9 * (seq_t)cand + (seq_t)8;   /* 9*p + 8 */

        if (is_prime_big(q))
        {
            grow_terms(terms_len + 1);
            terms[terms_len++] = q;
        }
    }
    /* last_checked becomes the largest p examined. */
    last_checked = cand - 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    generate_up_to(n);               /* fill cache if required */
    return terms[n - 1];              /* 1‑based index */
}
