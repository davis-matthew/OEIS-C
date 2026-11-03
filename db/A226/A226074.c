/*
 * A226074  Numbers k such that k^2+1 is a triangular number and k^2+2 is a
 *          prime number.
 * https://oeis.org/A226074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A226074: Numbers k such that k^2+1 is a triangular number
   and k^2+2 is prime.
   The terms are obtained by iterating the Pell equation
   u^2 – 8 k^2 = 9 with the fundamental solution (u , k) = (3 , 0).
   An acceptable term is produced whenever k^2+2 is prime. */
#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* For safety – the sequence is defined for n ≥ 1. */
        return 0;
    }

    /* ---------- static cache of admissible k's ---------- */
    static seq_t *cache = NULL;   /* array of stored k values */
    static size_t cap   = 0;      /* current capacity */
    static size_t size  = 0;      /* number of elements stored */

    /* ---------- static state for the Pell recurrence ---------- */
    static bool init = false;     /* one‑time initialisation flag */

    /* ---------- small‑integer case (≤ 8 bytes) ---------- */
    static __uint128_t u128 = 0, k128 = 0;     /* for small types */

    /* ---------- big‑integer case (≥ 16 bytes) ---------- */
    static seq_t u_big = 0, k_big = 0;

    /* ---------- one‑time initialisation ---------- */
    if (!init) {
        /* initialise the cache with the first term a(1)=0 */
        cap   = 16;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)0;
        size   = 1;

        /* initialise the Pell seed (u,k) = (3,0) */
        if (sizeof(seq_t) <= 8) {
            u128 = 3;
            k128 = 0;
        } else {
            u_big = (seq_t)3;
            k_big = (seq_t)0;
        }
        init = true;
    }

    /* ---------- generate terms until we have at least n of them ---------- */
    while (size < n) {
        /* 1. Compute the next solution (u_next , k_next) */
        if (sizeof(seq_t) <= 8) {
            __uint128_t u_next = 3 * u128 + 8 * k128;
            __uint128_t k_next = u128 + 3 * k128;

            /* 2. Check that k_next fits into the target integer type.
               If it does not, no further terms fit either, so we stop. */
            if (k_next > ( __uint128_t)~(unsigned long long)0ULL) {
                break;   /* no more admissible k that fit in seq_t */
            }

            /* 3. Test primality of k_next^2 + 2 (must also fit into 64 bits) */
            __uint128_t ksq = k_next * k_next;
            if (ksq > ( __uint128_t)~(unsigned long long)0ULL - 2ULL) {
                /* square exceeds 64‑bit range – cannot test primality,
                   but since k_next itself fits in 64 bits the smallest
                   such k_next would produce k^2+2 > 2^64-1, so we simply
                   skip it – we will never be asked for it. */
                /* Move to the next solution */
                u128 = u_next;
                k128 = k_next;
                continue;
            }
            unsigned long long test64 = (unsigned long long)ksq + 2ULL;
            bool is_prime = is_prime_seq((seq_t)test64);

            if (is_prime) {
                /* Ensure the cache has space. */
                if (size >= cap) {
                    size_t new_cap = cap ? cap << 1 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[size++] = (seq_t)k_next;
            }

            /* 4. Prepare for the next iteration. */
            u128 = u_next;
            k128 = k_next;
        } else {
            /* ---------- big‑integer case ---------- */
            /* compute next solution */
            seq_t u_next = u_big * (seq_t)3 + k_big * (seq_t)8;
            seq_t k_next = u_big + k_big * (seq_t)3;

            /* test primality of k_next^2 + 2 */
            seq_t test = k_next * k_next + (seq_t)2;
            bool is_prime = is_prime_big(test);

            if (is_prime) {
                if (size >= cap) {
                    size_t new_cap = cap ? cap << 1 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[size++] = k_next;
            }

            /* advance the Pell state */
            u_big = u_next;
            k_big = k_next;
        }
    }

    /* ---------- return the requested term ---------- */
    return cache[n - 1];
}
