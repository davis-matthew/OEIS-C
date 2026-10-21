/*
 * A059312  Primes p such that x^59 = 2 has no solution mod p.
 * https://oeis.org/A059312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A059312
 *  Primes p such that the congruence x^59 ≡ 2 (mod p) has no solution.
 *  A prime p satisfies this iff p ≡ 1 (mod 59) and 2^((p‑1)/59) ≠ 1 (mod p).
 *  The code searches primes in ascending order, testing only numbers
 *  congruent to 1 mod 59.  It memoizes the sequence so that calls with
 *  arbitrary indices are answered quickly.                                       */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *seq_terms = NULL;      /* array of computed terms */
static size_t seq_len   = 0;          /* number of terms stored */
static size_t seq_cap   = 0;          /* allocated capacity */

static idx_t next_k = 1;              /* next candidate index for 59*k+1 */

/* Ensure that seq_terms contains at least n terms. */
static void ensure_seq(idx_t n)
{
    /* if we already have enough terms, nothing to do */
    if (n <= seq_len) return;

    while (seq_len < n) {
        uint64_t cand = 59ull * (uint64_t)next_k + 1ull;  /* current candidate p */

        if (is_prime((idx_t)cand)) {                     /* is cand prime? */
            /* compute 2^((cand-1)/59) mod cand */
            uint64_t exp = (cand - 1ull) / 59ull;
            seq_t pow_res = mod_pow((seq_t)2, exp, (seq_t)cand);
            if (pow_res != (seq_t)1) {                   /* fails 59‑th power test */
                /* append to sequence */
                if (seq_len + 1 > seq_cap) {
                    size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
                    seq_terms = (seq_t *)realloc(seq_terms,
                                                 new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq_terms[seq_len++] = (seq_t)cand;
            }
        }
        ++next_k;                                         /* advance to next candidate */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for offset 0, return 0 */
    ensure_seq(n);
    return seq_terms[n - 1];           /* sequence is 1‑based */
}
