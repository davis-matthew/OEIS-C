/*
 * A040108  Primes p such that x^4 = 6 has no solution mod p.
 * https://oeis.org/A040108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 900 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iteratively testing primes for absence of a fourth‑root
   of 6 modulo the prime.  A prime p is accepted if 6^{(p−1)/gcd(4,p−1)}
   mod p ≠ 1.  The function lazily generates primes until it has found
   at least n accepted primes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/powmod.h"
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

/* --- internal helpers ---------------------------------------------- */

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- static state --------------------------------------------------- */

static seq_t *accepted = NULL;      /* array of primes that satisfy the property */
static size_t accepted_len = 0;
static size_t accepted_cap = 0;

static uint64_t last_checked = 1;   /* greatest integer that has been examined for primality */

/* Ensure we have at least target primes stored in accepted[] */
static void ensure_size(idx_t target)
{
    while (accepted_len < (size_t)target) {
        /* find next prime after last_checked */
        uint64_t cand = last_checked + 1;
        for (;;) {
            if (is_prime_big((seq_t)cand)) {
                break;
            }
            cand += 1;
        }

        /* test whether 6 has a fourth power modulo cand */
        if (cand > 3) {
            uint64_t g = gcd_uint64(4, cand - 1);
            uint64_t exp = (cand - 1) / g;
            seq_t chk = powmod((seq_t)6, exp, (seq_t)cand);
            if (chk != (seq_t)1) {
                append_candidate(&accepted, &accepted_len, &accepted_cap, (seq_t)cand);
            }
        }

        last_checked = cand;
    }
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    ensure_size(n);
    return accepted[(size_t)n - 1];
}
