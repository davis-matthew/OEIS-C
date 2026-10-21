/*
 * A216881  Primes p such that x^7 = 3 has a solution mod p.
 * https://oeis.org/A216881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which the congruence x^7 ≡ 3 (mod p)
 *            has a solution
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n) {
    /* Lazy static cache of qualifying primes */
    static seq_t *qual = NULL;
    static size_t qual_cnt = 0;
    static size_t qual_cap = 0;
    static uint64_t last_checked = 1;   /* prime examined up to last_checked */

    while (qual_cnt < n) {
        uint64_t p = next_prime(last_checked);
        last_checked = p;

        bool ok;
        if (p == 2 || p == 3 || (p % 7) != 1) {
            /* 7 does not divide p−1, so every element has a 7th power root */
            ok = true;
        } else {
            /* 7 | p−1, must test 3^((p−1)/7) ≡ 1 (mod p) */
            seq_t r = mod_pow((seq_t)3, (p - 1) / 7, (seq_t)p);
            ok = (r == (seq_t)1);
        }

        if (ok) {
            if (qual_cnt == qual_cap) {
                size_t new_cap = qual_cap == 0 ? 4 : qual_cap * 2;
                void *tmp = realloc(qual, new_cap * sizeof *qual);
                if (!tmp) abort();      /* out of memory */
                qual = (seq_t *)tmp;
                qual_cap = new_cap;
            }
            qual[qual_cnt++] = (seq_t)p;
        }
    }

    return qual[n - 1];   /* n is 1‑based */
}
