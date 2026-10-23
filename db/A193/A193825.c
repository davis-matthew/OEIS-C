/*
 * A193825  Primes of the form 10^n-prime(n).
 * https://oeis.org/A193825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^A110066(n) - prime(A110066(n))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   get_prime, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th term of OEIS sequence A193825.
 *  Uses a simple cache to avoid recomputing already found terms.
 *  The function works for any seq_t/idx_t provided by the build (built‑in
 *  integer or arbitrary precision) and for any order of calls. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

/* Cached sequence values */
static seq_t *seq_values = NULL;  /* array of previously found terms   */
static size_t  seq_len   = 0;     /* number of elements stored         */
static size_t  seq_cap   = 0;     /* current capacity of seq_values    */

/* State for incremental generation */
static idx_t next_k   = 1;            /* next exponent to test (k)        */
static seq_t pow_val   = (seq_t)1;    /* holds 10^(next_k-1)              */

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    while (seq_len < n) {
        /* Advance to the next power of ten: pow_val = 10^next_k */
        pow_val = mul_small(pow_val, 10);

        /* Retrieve the prime at index next_k */
        uint32_t pk32 = get_prime(next_k);
        seq_t   pk    = (seq_t)pk32;

        /* Candidate value: 10^next_k - prime(next_k) */
        seq_t cand = pow_val - pk;

        /* Test primality of the candidate */
        if (is_prime_big(cand)) {
            /* Resize cache if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 8 : seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq_values,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                seq_values = tmp;
                seq_cap    = new_cap;
            }
            seq_values[seq_len++] = cand;   /* store new term */
        }

        ++next_k;   /* advance to next exponent for the next iteration */
    }

    return seq_values[n - 1];
}
