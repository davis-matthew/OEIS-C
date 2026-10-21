/*
 * A096292  Primes p such that p!-1 is divisible by the next prime larger than
 *          p.
 * https://oeis.org/A096292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 600, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = p where p is a prime such that (p! - 1) mod next_prime(p) == 0.
 * Implementation: For each prime p, let q = next_prime(p).
 *   By Wilson's theorem, (q-1)! ≡ -1 (mod q).
 *   Since (q-1)! = p! · Π_{k=p+1}^{q-1} k (mod q),
 *   we have p! ≡ 1 (mod q) ⇔ Π_{k=p+1}^{q-1} k ≡ -1 (mod q).
 *   Therefore it suffices to multiply only the numbers between p+1 and q-1 (a very short range) modulo q.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

static seq_t *seq_vals   = NULL;  /* stored terms of the sequence */
static size_t seq_cap    = 0;      /* allocated capacity of seq_vals   */
static size_t seq_len    = 0;      /* number of terms currently stored */
static uint64_t last_prime = 2;    /* last prime that has been examined */
static bool initialized   = false;

seq_t a(idx_t n)
{
    /* Ensure one-time initialization */
    if (!initialized) {
        seq_vals   = NULL;
        seq_cap    = 0;
        seq_len    = 0;
        last_prime = 2;
        initialized = true;
    }

    /* Generate terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* Find the next prime after the current one */
        last_prime = next_prime(last_prime);
        uint64_t q = next_prime(last_prime);    /* next prime after p */

        /* Compute the product of the integers from p+1 to q-1 modulo q */
        seq_t prod = 1;                         /* integer type sufficient for small q */
        for (uint64_t x = last_prime + 1; x <= q - 1; ++x) {
            prod = (prod * (seq_t)x) % (seq_t)q;
        }

        /* Check the condition Π ≡ -1 (mod q), i.e. prod == q-1 */
        if (prod == (seq_t)(q - 1)) {
            /* Ensure capacity for the new term */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)last_prime;
        }
    }

    /* Return the requested term (1‑based indexing) */
    return seq_vals[n - 1];
}
