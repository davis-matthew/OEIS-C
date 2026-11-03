/*
 * A076518  Numbers n such that Fibonacci(n) == sigma(n) (mod n).
 * https://oeis.org/A076518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9795 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076518 – Numbers n such that Fibonacci(n) ≡ σ(n) (mod n)
   The function a(n) returns the n‑th such integer (1‑based indexing). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* fast doubling: returns F(n) mod mod and F(n+1) mod mod */
static void fib_pair_mod(unsigned long long n, unsigned long long mod,
                         unsigned long long *fn, unsigned long long *fn1)
{
    if (n == 0ULL) {
        *fn = 0ULL;
        *fn1 = 1ULL;
        return;
    }
    unsigned long long a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    unsigned long long t1 = (unsigned long long)(
        (unsigned __int128)a * ((2ULL * b % mod + mod - a) % mod) % mod);
    unsigned long long t2 = (unsigned long long)(
        ((unsigned __int128)a * a + (unsigned __int128)b * b) % mod);
    if ((n & 1ULL) == 0ULL) {
        *fn = t1;
        *fn1 = t2;
    } else {
        *fn = t2;
        *fn1 = (t1 + t2) % mod;
    }
}

/* Fibonacci(n) mod mod (with F(0)=0, F(1)=1). Handles mod==1 correctly. */
static unsigned long long fib_mod(unsigned long long n, unsigned long long mod)
{
    if (mod == 1ULL) return 0ULL;
    unsigned long long fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn % mod;
}

/* σ(n) mod n. n >= 1. */
static unsigned long long sigma_mod(unsigned long long n)
{
    if (n == 1ULL) return 1ULL % n;  /* σ(1)=1 */
    unsigned long long rem = n;
    unsigned long long result = 1ULL % n;

    /* factor 2 */
    unsigned int exp = 0;
    while ((rem & 1ULL) == 0ULL) {
        ++exp;
        rem >>= 1;
    }
    if (exp) {
        /* term = (2^(exp+1)-1)/(2-1) = 2^(exp+1)-1 */
        unsigned __int128 pow = 1;
        for (unsigned int i = 0; i < exp + 1; ++i) pow *= 2;
        pow -= 1;
        unsigned long long term = (unsigned long long)(pow % n);
        result = (unsigned long long)((unsigned __int128)result * term % n);
    }

    /* odd factors */
    for (unsigned long long p = 3ULL; p * p <= rem; p += 2ULL) {
        if (rem % p == 0ULL) {
            exp = 0;
            while (rem % p == 0ULL) {
                ++exp;
                rem /= p;
            }
            /* term = (p^(exp+1)-1)/(p-1) */
            unsigned __int128 num = 1;
            for (unsigned int i = 0; i < exp + 1; ++i) num *= p;
            num -= 1;
            unsigned __int128 term128 = num / (p - 1);
            unsigned long long term = (unsigned long long)(term128 % n);
            result = (unsigned long long)((unsigned __int128)result * term % n);
        }
    }

    /* remaining prime > 1 */
    if (rem > 1ULL) {
        unsigned long long term = (1ULL + rem) % n; /* p^(1+1)-1)/(p-1) = 1+p */
        result = (unsigned long long)((unsigned __int128)result * term % n);
    }

    return result % n;
}

/* check property for a given n */
static bool property_holds(unsigned long long n)
{
    unsigned long long fmod = fib_mod(n, n);
    unsigned long long mod = sigma_mod(n);
    return fmod == mod;
}

/* dynamic storage of terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t tested_n = 0;

/* ensure capacity for terms vector */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t newcap = terms_cap == 0 ? 64 : terms_cap * 2;
    while (newcap < needed) newcap *= 2;
    terms = mem_resize(seq_t, terms, newcap);
    terms_cap = newcap;
}

/* main entry */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (terms_len < n) {
        ++tested_n;
        if (property_holds(tested_n)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)tested_n;
        }
    }
    return terms[n - 1];
}
