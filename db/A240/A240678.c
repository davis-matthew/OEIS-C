/*
 * A240678  Primes p such that p*10+k is prime for exactly one value of the
 *          digit k.
 * https://oeis.org/A240678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 307, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A240678 sequence.
   For each prime p, count how many numbers p*10+k (k = 0..9) are prime.
   Exactly one such number must be prime to include p in the sequence. */
/* The function returns the n-th term (1‑based). The sequence is built
   incrementally and cached so that calls with arbitrary n work. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found sequence terms */
    static seq_t *buf   = NULL;
    static size_t buf_cap = 0;
    static size_t buf_cnt = 0;
    /* Next integer to test for primality (odd numbers only after 2) */
    static uint64_t next_candidate = 2;

    if (n == 0) return (seq_t)0;  /* undefined but safe guard */

    /* Keep searching until we have at least n terms in the cache */
    while (buf_cnt < n) {
        /* Find the next prime candidate */
        uint64_t cand = next_candidate;
        if (cand < 2) cand = 2;
        while (!is_prime_big((seq_t)cand)) {
            /* Skip even numbers after 2 */
            cand += (cand == 2 ? 1 : 2);
        }

        /* cand is a prime */
        seq_t p = cand;

        /* Count how many of p*10+k are prime for k=0..9 */
        int prime_digit_count = 0;
        for (int d = 0; d <= 9; ++d) {
            seq_t num = p * 10 + d;
            if (is_prime_big(num)) {
                ++prime_digit_count;
                if (prime_digit_count > 1) break;
            }
        }

        /* If exactly one digit works, store this prime */
        if (prime_digit_count == 1) {
            if (buf_cnt >= buf_cap) {
                size_t new_cap = (buf_cap == 0) ? 16 : buf_cap * 2;
                /* Realloc must be cast for C++ compatibility */
                seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failure */
                buf = tmp;
                buf_cap = new_cap;
            }
            buf[buf_cnt++] = p;
        }

        /* Next candidate to test (odd numbers only) */
        next_candidate = (cand == 2) ? 3 : cand + 2;
    }

    return buf[n - 1];
}
