/*
 * A063877  Composite numbers not divisible by 7 which in base 7 contain their
 *          largest proper factor as a substring.
 * https://oeis.org/A063877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mem, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth composite number not divisible by 7
 * whose base–7 representation contains its largest proper divisor
 * as a contiguous substring.  The value is found by a search over
 * increasing integers, rejecting primes, numbers divisible by 7,
 * and testing the substring property.  Factorisation uses a
 * deterministic Miller–Rabin test for primality and a Pollard–Rho
 * algorithm to obtain the smallest prime factor (from which the
 * largest proper factor is derived).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* convert a non‑negative integer to base‑7 string (null‑terminated) */
static void to_base7(uint64_t n, char *out) {
    int i = 0;
    do {
        out[i++] = (char)('0' + (n % 7));
        n /= 7;
    } while (n);
    out[i] = '\0';
    /* reverse string in place */
    for (int l = 0, r = i - 1; l < r; ++l, --r) {
        char t = out[l];
        out[l] = out[r];
        out[r] = t;
    }
}

/* small auxiliary arithmetic helpers */

/* dynamic array to hold computed terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 4;   /* start checking from 4 */

/* ensure dynamic array has room for at least 'need' elements */
static void ensure_capacity(size_t need) {
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    if (!tmp) abort(); /* allocation failed */
    terms = tmp;
    terms_cap = new_cap;
}

/* push a new term into the dynamic array */
static void push_term(seq_t val) {
    if (terms_len == terms_cap) {
        ensure_capacity(terms_len + 1);
    }
    terms[terms_len++] = val;
}

/* main function requested by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */
    /* generate terms until we have at least n */
    while (terms_len < (size_t)n) {
        /* find next candidate */
        uint64_t cand = next_candidate++;
        if (cand % 7 == 0) continue;                 /* skip multiples of 7 */
        if (is_prime_u64(cand)) continue;        /* skip primes */

        uint64_t spf = smallest_prime_factor(cand);  /* smallest prime factor */
        uint64_t largest = cand / spf;               /* largest proper factor */

        /* check substring condition in base 7 */
        char numbuf[32], largbuf[32];
        to_base7(cand, numbuf);
        to_base7(largest, largbuf);
        if (strstr(numbuf, largbuf) != NULL) {
            push_term((seq_t)cand);
        }
    }
    return terms[n - 1];
}
