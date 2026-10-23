/*
 * A164114  Numbers k such that Chowla(k) + phi(k) is prime.
 * https://oeis.org/A164114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1949, u32 n <= 3612, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164114: a(n) = the nth integer k such that A048050(k)+phi(k) is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Storage for the computed sequence */
#if defined(__cplusplus)
    static seq_t *seq = nullptr;
#else
    static seq_t *seq = NULL;
#endif
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* Current integer being tested */
    static idx_t next_k = 1;          /* start after 1 */

    /* Smallest prime factor table for fast factorisation */
#if defined(__cplusplus)
    static uint64_t *spf = nullptr;
#else
    static uint64_t *spf = NULL;
#endif
    static size_t spf_cap = 0;

    /* Helper to extend the spf table up to a given limit */
    if (n == 0) return 0; /* not required but safe */

    while (seq_len < n) {
        /* Increase the integer to test */
        next_k++;

        /* Ensure spf table covers 'next_k' */
        if ((size_t)next_k > spf_cap) {
            size_t new_cap = spf_cap ? spf_cap : 16;
            while ((size_t)next_k > new_cap) new_cap <<= 1;

            uint64_t *new_spf = (uint64_t*)calloc(new_cap + 1, sizeof(uint64_t));
            /* Build spf up to new_cap */
            new_spf[0] = 0;
            if (new_cap >= 1) new_spf[1] = 1;
            for (size_t i = 2; i <= new_cap; ++i) {
                if (new_spf[i] == 0) {
                    new_spf[i] = i;
                    if ((uint64_t)i * i <= new_cap) {
                        for (size_t j = i * i; j <= new_cap; j += i) {
                            if (new_spf[j] == 0) new_spf[j] = i;
                        }
                    }
                }
            }
            free(spf);
            spf = new_spf;
            spf_cap = new_cap;
        }

        /* Factorisation of next_k using spf */
        idx_t k = next_k;
        seq_t sigma = 1;          /* sum of all divisors */
        seq_t phi_val = k;        /* Euler's totient */
        idx_t m = k;
        while (m > 1) {
            uint64_t p = spf[m];
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* Compute (1 + p + ... + p^exp) */
            seq_t sum = 1;
            seq_t p_pow = 1;
            for (int i = 0; i < exp; ++i) {
                p_pow *= (seq_t)p;
                sum += p_pow;
            }
            sigma *= sum;
            /* Update phi */
            phi_val = phi_val / p * (p - 1);
        }

        seq_t sum_nontrivial = sigma - (seq_t)k - 1;
        seq_t s_total = sum_nontrivial + phi_val;

        if (is_prime_big(s_total)) {
            /* Grow sequence array if needed */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
#if defined(__cplusplus)
                seq_t *new_arr = new seq_t[new_cap];
                for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq[i];
                delete[] seq;
                seq = new_arr;
#else
                seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
#endif
                seq_cap = new_cap;
            }
            seq[seq_len++] = k;
        }
    }

    return seq[n - 1];
}
