/*
 * A188159  Odd numbers k such that 1^((k-1)/2) + 2^((k-1)/2) + ... +
 *          (k-1)^((k-1)/2) <> 0 (mod k).
 * https://oeis.org/A188159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 7921, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188159: a(n) is the n‑th odd integer k such that
 *  Σ_{i=1}^{k-1} i^{(k-1)/2} ≢ 0 (mod k).
 * Computed by scanning odd k and testing whether, for at least one
 * prime divisor p of k, (k-1)/2 is divisible by (p-1).  This
 * condition follows from the fact that for a prime p, the sum
 * of the (k-1)/2‑th powers modulo p is zero unless (p–1) divides
 * the exponent, in which case the sum is −1 modulo p.  A prime
 * factor giving a non‑zero residue guarantees the whole sum is
 * non‑zero modulo k.  The function returns terms in increasing
 * order and memoises results across calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t next_k = 3;          /* first odd k to test   */

    while (len < n) {
        /* Scan odd k starting from the last inspected value.  */
        for (idx_t k = next_k; ; k += 2) {
            idx_t m = (k - 1) / 2;          /* exponent (k‑1)/2   */
            idx_t tmp = k;                  /* copy for factorisation */
            bool good = false;

            /* Check prime divisors p >= 3. */
            for (idx_t p = 3; p <= tmp / p; p += 2) {
                if (tmp % p == 0) {
                    if (m % (p - 1) == 0) {
                        good = true;
                        break;
                    }
                    /* Remove all occurrences of p. */
                    while (tmp % p == 0) {
                        tmp /= p;
                    }
                }
            }
            if (!good && tmp > 1) {
                /* Remaining factor is prime. */
                if (m % (tmp - 1) == 0) {
                    good = true;
                }
            }

            if (good) {
                /* Store the found term. */
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 8;
                    seq_t *new_buf = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                    if (!new_buf) return 0;          /* allocation failed */
                    cache = new_buf;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)k;
                next_k = k + 2;                     /* next odd to try */
                if (len == n) {
                    return cache[n - 1];
                }
                break;  /* proceed to next term */
            }
        }
    }
    return cache[n - 1];
}
