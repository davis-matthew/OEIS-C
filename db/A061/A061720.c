/*
 * A061720  First differences of sequence of primorials.
 * https://oeis.org/A061720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002110(n+1) – A002110(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---------- helper that generates successive primes ----------
 * Uses simple trial‑division, caching primes in a static array.
 * All state is static, so callers may invoke a() out of order.
 */
static uint32_t _next_prime(void) {
    static uint32_t last_prime = 0;          /* 0 means no prime yet */
    static uint32_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    uint32_t cand, i, p;

    /* first prime is 2 */
    if (last_prime == 0) {
        last_prime = 2;
        if (prime_cap == 0) {
            prime_cap = 8;
            prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        }
        prime_list[prime_cnt++] = 2;
        return 2;
    }

    cand = last_prime + 1;
    for (;;) {
        /* skip even numbers */
        if ((cand & 1U) == 0U) { cand++; continue; }

        int ok = 1;
        for (i = 0; i < prime_cnt; i++) {
            p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { ok = 0; break; }
        }
        if (ok) {
            last_prime = cand;
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = cand;
            return cand;
        }
        cand++;
    }
}

/* ---------- sequence a(n) ----------
 * Computes a(n) = primorial(n+1) – primorial(n).
 * Cache primorials and a(n) statically to support arbitrary call order.
 */
seq_t a(idx_t n) {
    static seq_t *primorial = NULL;  /* primorial[0] = 1, primorial[1] = 2, ... */
    static size_t prim_size = 0, prim_cap = 0;

    static seq_t *a_arr = NULL;      /* a_arr[0] = a(0), a_arr[1] = a(1), ... */
    static size_t a_size = 0, a_cap = 0;

    /* one‑time initialization */
    if (primorial == NULL) {
        prim_cap = 8;
        primorial = mem_new(seq_t, prim_cap);
        primorial[0] = (seq_t)1;          /* primorial(0) = 1 */
        prim_size = 1;

        a_cap = 8;
        a_arr = mem_new(seq_t, a_cap);
        a_size = 0;
    }

    /* Generate primorials up to index n+1, and the corresponding a(n) */
    while (prim_size <= (size_t)n + 1) {
        uint32_t p = _next_prime();           /* next prime */
        seq_t prev = primorial[prim_size - 1];
        seq_t cur  = prev * (seq_t)p;         /* new primorial */

        /* grow primorial array if needed */
        if (prim_size == prim_cap) {
            prim_cap <<= 1;
            primorial = mem_resize(seq_t, primorial, prim_cap);
        }
        primorial[prim_size++] = cur;

        /* compute a[prim_size-2] = cur – prev */
        if (a_size == a_cap) {
            a_cap <<= 1;
            a_arr = mem_resize(seq_t, a_arr, a_cap);
        }
        a_arr[a_size++] = cur - prev;
    }

    return a_arr[(size_t)n];
}
