/*
 * A126769  Primes p of the form k^4 + s where k > 1, s >= 1 and k^2 + s is
 *          also prime.
 * https://oeis.org/A126769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 5075, u32 n <= 5275, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126769: primes p such that p = k^4 + s (k>1, s≥1)
   and k^2 + s is also prime.  The i‑th term a(i) is the i‑th such prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* ---------------------------------------------------------------------- */
/*  Static storage for the computed sequence                              */
/* ---------------------------------------------------------------------- */
static seq_t *seq_arr = NULL;   /* dynamic array to store the terms      */
static size_t seq_len = 0;      /* current number of terms stored        */
static size_t seq_cap = 0;      /* capacity of seq_arr                    */

static uint64_t next_candidate = 17;  /* next integer to test (primes ≥ 17)    */

/* ---------------------------------------------------------------------- */
/*  Ensure that at least n terms have been generated.                     */
/* ---------------------------------------------------------------------- */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= n)
        return;

    uint64_t cand = next_candidate;

    while (seq_len < n) {
        seq_t cand_val = (seq_t)cand;

        if (is_prime_big(cand_val)) {             /* check primality of p   */
            bool ok = false;

            /* Try all possible k > 1 with k^4 < p */
            for (int k = 2; ; ++k) {
                seq_t ksq = (seq_t)k * k;          /* k^2                  */
                seq_t k4  = ksq * ksq;             /* k^4                  */

                if (k4 >= cand_val)                /* no larger k possible */
                    break;

                seq_t s = cand_val - k4;           /* s = p - k^4           */
                seq_t q = ksq + s;                 /* q = k^2 + s           */

                if (is_prime_big(q)) {             /* check q primality     */
                    ok = true;
                    break;
                }
            }

            if (ok) {                            /* store the term */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_arr = (seq_t *)realloc(seq_arr,
                                                seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = cand_val;
            }
        }
        ++cand;
    }
    next_candidate = cand;                       /* remember where we stopped */
}

/* ---------------------------------------------------------------------- */
/*  Return the n‑th term of the sequence                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0, but harmless */

    ensure_sequence(n);
    return seq_arr[n - 1];
}
