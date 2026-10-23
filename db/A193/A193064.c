/*
 * A193064  Primes p for which there is no prime q == 2 (mod 3) that is
 *          smaller than p and is a quadratic residue modulo p.
 * https://oeis.org/A193064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  For each prime p, test whether there exists a
   smaller prime q ≡ 2 (mod 3) that is a quadratic residue modulo p.
   If no such q exists, store p in the sequence.  a(1) = 2,
   a(2) = 3, a(3) = 5, a(4) = 13, … */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *seq_arr   = NULL;   /* list of sequence terms */
static size_t   seq_len = 0, seq_cap = 0;

static seq_t *pr2_arr   = NULL;   /* primes q ≡ 2 (mod 3) */
static size_t   pr2_len = 0, pr2_cap = 0;

static uint64_t last_prime = 0;   /* last prime produced by next_prime */

/* Ensure capacity of a dynamic array of seq_t */
static void grow_arr(seq_t **arr, size_t *cap, size_t *len)
{
    if (*len >= *cap) {
        *cap = (*cap == 0) ? 8 : (*cap << 1);
        #ifdef __cplusplus
        *arr = static_cast<seq_t*>(realloc(*arr, *cap * sizeof(seq_t)));
        #else
        *arr = (seq_t*)realloc(*arr, *cap * sizeof(seq_t));
        #endif
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Next prime p */
        uint64_t next = next_prime(last_prime);
        last_prime = next;
        seq_t p = (seq_t)next;

        /* Keep all primes ≡ 2 (mod 3) for later checks */
        if (next % 3 == 2) {
            grow_arr(&pr2_arr, &pr2_cap, &pr2_len);
            pr2_arr[pr2_len++] = p;
        }

        /* Test the condition */
        bool ok = true;
        uint64_t exp = (next - 1) / 2u;           /* (p-1)/2 */
        for (size_t i = 0; i < pr2_len; ++i) {
            seq_t q = pr2_arr[i];
            if (q >= p) break;                  /* only smaller primes */
            seq_t leg = mod_pow(q, exp, p);     /* Legendre (q|p) */
            if (leg == (seq_t)1) {               /* quadratic residue found */
                ok = false;
                break;
            }
        }

        if (ok) {                               /* p ∈ A193064 */
            grow_arr(&seq_arr, &seq_cap, &seq_len);
            seq_arr[seq_len++] = p;
        }
    }

    return seq_arr[(size_t)n - 1];
}
