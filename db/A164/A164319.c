/*
 * A164319  Primes p such that the sum of divisors of p+1 is larger than 2*p.
 * https://oeis.org/A164319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1508, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 72 terms through n = 4406 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164319: primes p such that σ(p+1) > 2p (search by primes) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* static storage for memoisation */
static seq_t *stored_primes = NULL;      /* array of found primes      */
static size_t stored_len   = 0;          /* number of primes stored   */
static size_t stored_cap   = 0;          /* allocated capacity       */
static idx_t  next_candidate = 2;        /* next integer to test     */

/* Return the n‑th term (1‑based) of A164319 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defensive, undefined */

    while (stored_len < n) {
        idx_t p = 0;

        /* find the next prime */
        for (;;) {
            if (next_candidate <= 2) {
                p = 2;
                next_candidate = 3;
            } else {
                /* skip even numbers */
                if ((next_candidate & 1) == 0) {
                    ++next_candidate;
                    continue;
                }
                p = next_candidate;
                ++next_candidate;
            }
            if (is_prime(p)) break;       /* p is prime */
        }

        /* compute σ(p+1) and compare with 2p */
        seq_t sigma = sigma_of(p + 1);
        seq_t twoP   = (seq_t)p;
        twoP       *= 2;

        if (sigma > twoP) {
            /* grow storage if needed */
            if (stored_len == stored_cap) {
                size_t new_cap = stored_cap ? stored_cap * 2 : 16;
                grow_seq_array(&stored_primes, &stored_cap, new_cap);
            }
            stored_primes[stored_len++] = (seq_t)p;
        }
    }

    return stored_primes[n - 1];
}
