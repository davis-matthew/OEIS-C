/*
 * A155007  Primes p such that (p-3)*(p+3)-+3*p are primes.
 * https://oeis.org/A155007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that (p-3)*(p+3)-3p and (p-3)*(p+3)+3p are primes
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 6, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *seq = NULL;       /* stored Sequence values */
static size_t seq_len = 0;      /* current number of stored terms */
static size_t seq_cap = 0;      /* capacity of the seq array */
static seq_t next_candidate = 2;/* next integer to test for being a prime */

/* Ensure the sequence contains at least n elements (1‑based) */
static void generate_upto(idx_t n)
{
    if (seq_len >= (size_t)n) return;          /* already have enough */

    /* initialize storage if first call */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    seq_t p = next_candidate; /* start from the last used candidate */

    while (seq_len < (size_t)n) {
        /* Find next prime candidate */
        while (!is_prime_big(p)) ++p;

        /* p is prime, compute the two expressions */
        seq_t p2 = p * p;
        seq_t a  = p2 + 3 * p - 9; /* (p-3)*(p+3)-3p = p^2-3p-9 */
        seq_t b  = p2 - 3 * p - 9; /* (p-3)*(p+3)+3p = p^2+3p-9 */

        if (is_prime_big(a) && is_prime_big(b)) {
            /* store p in the sequence */
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = p;
        }
        ++p; /* move to next integer */
    }

    next_candidate = p; /* remember where we left off */
}

/* External linkage : returns the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n==0, return 0 */
    generate_upto(n);
    return seq[n - 1];
}
