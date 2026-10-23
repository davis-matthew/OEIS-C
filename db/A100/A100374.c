/*
 * A100374  Largest power of 2 dividing prime(n+1) - prime(n), the n-th
 *          consecutive prime difference.
 * https://oeis.org/A100374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 dividing prime(n+1) – prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_lowbit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2_lowbit.h"

/* dynamic storage for primes, 0‑based */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;
static uint64_t last_checked = 1;

/* Ensure prime_list contains at least idx+1 primes (idx >= 0). */
static void ensure_primes(size_t idx) {
    while (prime_cnt <= idx) {
        uint64_t cand = ++last_checked;
        if (cand < 2) continue;
        int is_prime = 1;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* OEIS sequence A100374 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* defined for safety */
    ensure_primes((size_t)n);               /* need primes up to prime(n+1) */
    uint64_t diff = prime_list[n] - prime_list[n-1];
    return pow2_lowbit(diff);
}
