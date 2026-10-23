/*
 * A049578  Primes p such that x^46 = 2 has a solution mod p.
 * https://oeis.org/A049578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049578: primes p such that x^46 = 2 has a solution mod p.
   The code tests successive primes for the condition
   2^((p-1)/gcd(46,p-1)) ≡ 1 (mod p).                         */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"   /* mod_pow(b,e,m)  - works for all seq_t types */
#include "../utils/gcd_u64.h"

/* Euclidean gcd for 64‑bit unsigned integers */

/* ------------------------------------------------------------------ */
/* Sequence generator                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Cache of computed terms                                          */
    /* ------------------------------------------------------------------ */
    static seq_t *cache  = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* ------------------------------------------------------------------ */
    /*  Prime list used for trial division                                */
    /* ------------------------------------------------------------------ */
    static uint64_t *prime_list = NULL;
    static size_t primes_len = 0, primes_cap = 0;

    /* ------------------------------------------------------------------ */
    /*  Current candidate for the next prime                              */
    /* ------------------------------------------------------------------ */
    static uint64_t cand = 2;

    /* Allocate initial space for cache and prime list if necessary */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (primes_cap == 0) {
        primes_cap = 128;
        prime_list = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* ------------------------------------------------------------------ */
    /*  Generate terms until we have at least n of them                    */
    /* ------------------------------------------------------------------ */
    while (cache_len < n) {
        uint64_t p = 0;

        /* ------------------------------------------------------------------ */
        /*  Find next prime >= cand                                           */
        /* ------------------------------------------------------------------ */
        uint64_t cur = cand;
        if (cur == 2) {
            p = 2;
            cur = 3;          /* next candidate after 2 is 3 (odd) */
        } else {
            if (cur % 2 == 0)
                ++cur;        /* make odd */
            for (;;) {
                bool prime = true;
                for (size_t i = 0; i < primes_len; ++i) {
                    uint64_t q = prime_list[i];
                    if (q * q > cur)
                        break;
                    if (cur % q == 0) {
                        prime = false;
                        break;
                    }
                }
                if (prime) {
                    p = cur;
                    cur += 2;  /* next candidate after the found prime */
                    break;
                }
                cur += 2;      /* test next odd number */
            }
        }
        cand = cur;        /* prepare for next call */

        /* ------------------------------------------------------------------ */
        /*  Store the discovered prime for future trial divisions            */
        /* ------------------------------------------------------------------ */
        if (primes_len == primes_cap) {
            primes_cap = primes_cap ? primes_cap * 2 : 4;
            prime_list = (uint64_t *)realloc(prime_list, primes_cap * sizeof(uint64_t));
        }
        prime_list[primes_len++] = p;

        /* ------------------------------------------------------------------ */
        /*  Test the 46‑th power residue condition                            */
        /* ------------------------------------------------------------------ */
        bool ok = false;
        if (p == 2) {
            ok = true; /* 0^46 ≡ 2 (mod 2) */
        } else {
            uint64_t g = gcd_u64(46ULL, p - 1ULL);
            uint64_t e = (p - 1ULL) / g;
            seq_t r = mod_pow((seq_t)2, e, (seq_t)p);
            ok = (r == (seq_t)1);
        }

        /* ------------------------------------------------------------------ */
        /*  Store in cache if condition satisfied                            */
        /* ------------------------------------------------------------------ */
        if (ok) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
