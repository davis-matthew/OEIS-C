/*
 * A154608  Primes p such that 11*p + 4 is also prime.
 * https://oeis.org/A154608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the primes p such that 11*p + 4 is also prime (OEIS A154608). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

static seq_t *qual = NULL;      /* array of qualifying primes */
static size_t qual_cap = 0;     /* current capacity of the array */
static size_t qual_len = 0;     /* number of items stored so far */
static idx_t last_checked = 1;  /* the last number examined for the next prime */

/* Ensure that qual[] contains at least n qualifying primes. */
static void ensure_n(idx_t n)
{
    while (qual_len < (size_t)n) {
        /* Find the next prime after the last checked number. */
        uint64_t p = next_prime((uint64_t)last_checked);
        last_checked = (idx_t)p;

        /* Check whether 11*p + 4 is also prime. */
        seq_t m = (seq_t)p * (seq_t)11 + (seq_t)4;
        if (is_prime_big(m)) {
            /* Grow the array if necessary. */
            if (qual_len >= qual_cap) {
                size_t newcap = qual_cap ? (qual_cap << 1) : 64;
                seq_t *tmp = mem_resize(seq_t, qual, newcap);
                if (!tmp) abort();
                qual = tmp;
                qual_cap = newcap;
            }
            qual[qual_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* below offset, undefined behaviour */
    ensure_n(n);
    return qual[(size_t)(n - 1)];
}
