/*
 * A139242  Second differences of Mersenne numbers A001348, divided by 2.
 * https://oeis.org/A139242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^{p_{n+2}} - 2^{p_{n+1}+1} + 2^{p_n}) / 2 where p_k is
 *            the k‑th prime. This equals (M_{n+2} - 2*M_{n+1} + M_n) / 2 with
 *            M_k = 2^{p_k} - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* --- Prime cache ---------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Ensure at least 'needed' primes are stored in prime_list. */
static void ensure_primes(idx_t needed) {
    if (needed <= prime_cnt)
        return;

    /* Grow array capacity if necessary */
    if (prime_cap < needed) {
        prime_cap = (prime_cap > 0) ? prime_cap * 2 : needed * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) abort();   /* allocation failure */
        prime_list = tmp;
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    while (prime_cnt < needed) {
        bool is_prime = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)          /* stop when p² > candidate */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
        }

        ++candidate;
    }
}

/* --- OEIS function --------------------------------------- */
seq_t a(idx_t n) {
    /* We need p_n, p_{n+1} and p_{n+2}. */
    ensure_primes(n + 2);

    uint64_t pn  = prime_list[n - 1];   /* p_n   (n starts at 1) */
    uint64_t pn1 = prime_list[n];       /* p_{n+1} */
    uint64_t pn2 = prime_list[n + 1];   /* p_{n+2} */

    /* Compute 2^{p_{n+2}}, 2^{p_{n+1}+1} and 2^{p_n}. */
    seq_t term1 = pow2((idx_t)pn2);            /* 2^{p_{n+2}} */
    seq_t term2 = pow2((idx_t)(pn1 + 1));      /* 2^{p_{n+1}+1} = 2 * 2^{p_{n+1}} */
    seq_t term3 = pow2((idx_t)pn);             /* 2^{p_n} */

    /* (term1 - term2 + term3) / 2 */
    return (term1 - term2 + term3) / (seq_t)2;
}
