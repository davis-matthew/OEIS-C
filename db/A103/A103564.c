/*
 * A103564  Primes p such that 3*p^2 + 2 is prime.
 * https://oeis.org/A103564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   by testing increasing primes p for the condition 3*p^2+2 prime
 * Range:     u8 n <= 7, u16 n <= 502, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t term_cap = 0;   /* allocated capacity */
    static idx_t term_cnt = 0;   /* terms currently stored */
    static uint64_t next_cand = 3;   /* next odd candidate to test */

    if (term_cnt < n) {
        /* enlarge storage if needed */
        if (term_cap < n) {
            idx_t new_cap = term_cap ? term_cap << 1 : 16;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc((void *)terms, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* OOM – unlikely in tests */
            terms = tmp;
            term_cap = new_cap;
        }

        /* generate new qualifying primes until we have n terms */
        while (term_cnt < n) {
            uint64_t p = next_cand;
            for (;;) {
                if (is_prime(p)) {
                    /* compute 3*p^2 + 2 */
                    unsigned __int128 sq   = (unsigned __int128)p * p;
                    unsigned __int128 val128 = 3 * sq + 2;
                    uint64_t val = (uint64_t)val128;   /* fits in 64‑bit here */
                    if (is_prime(val)) {
                        terms[term_cnt++] = (seq_t)p;
                    }
                    next_cand = p + 2;   /* advance to next odd candidate */
                    break;
                }
                p += 2; /* skip even numbers */
            }
        }
    }

    /* n is 1‑based; return the (n‑1)th element */
    return terms[n - 1];
}
