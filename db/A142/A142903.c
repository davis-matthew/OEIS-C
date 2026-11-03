/*
 * A142903  Primes congruent to 25 mod 63.
 * https://oeis.org/A142903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 25 (mod 63) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/*  a(n) = the nth prime p such that p ≡ 25 (mod 63)  (n ≥ 1)  */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *terms      = NULL;   /* stored primes */
    static size_t  terms_cap = 0;      /* allocated capacity */
    static idx_t   terms_cnt = 0;      /* number of primes stored */
    static idx_t   cand       = 0;      /* current candidate (∈ 25 + 63·ℕ) */
    static bool    init       = false; /* initialization flag */

    /* initialise on first call */
    if (!init) {
        init = true;
        terms_cap = 64;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) return (seq_t)0;              /* allocation failure */
        terms_cnt = 0;
        cand = (idx_t)63 * 2 + 25;                /* first potential prime */
    }

    /* if already have enough terms, return the required one */
    if (terms_cnt >= n) {
        if (n == 0) return (seq_t)0;              /* out of bounds, but safe */
        return terms[n - 1];
    }

    /* generate further terms until we reach the requested index */
    while (terms_cnt < n) {
        if (is_prime_big((seq_t)cand)) {
            /* grow the array if needed */
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                if (!tmp) return (seq_t)0;        /* reallocation failure */
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)cand;
        }
        cand += (idx_t)63;  /* next number in the progression 25 + 63·k */
    }

    return terms[n - 1];
}
