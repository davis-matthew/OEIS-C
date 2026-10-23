/*
 * A103888  Numbers k such that the string 100k001 is prime.
 * https://oeis.org/A103888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_seq, pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103888 – numbers k such that 100k001 is prime.
   The function builds the sequence lazily by searching k = 1,2,3,...
   A cache of found terms is kept so that subsequent calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digits.h"
#include "../utils/pow10_idx.h"
#include "../utils/is_prime_seq.h"

/* Cached values of a(n).  terms[i] holds a(i+1). */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;   /* allocated capacity */
static size_t terms_len  = 0;   /* current number of stored terms */

/* Last candidate k that was examined.  k starts at 0 so the first
   candidate will be 1. */
static idx_t last_k = 0;

/* Construct the concatenation 100k001 as a seq_t.  Let d be the number of
   decimal digits of k.  The number has the form
          100 * 10^(d+3) + k * 1000 + 1
   which fits into seq_t for any k that is checked. */
static seq_t make_N(idx_t k)
{
    unsigned int d = digits(k);        /* number of decimal digits of k */
    seq_t pow10 = pow10_idx(d + 3);    /* 10^(d+3) */
    seq_t N = (seq_t)100 * pow10;      /* 100 * 10^(d+3) */
    N += (seq_t)k * (seq_t)1000;       /* + k * 1000 */
    N += (seq_t)1;                     /* + 1 */
    return N;
}

/* External linkage per the build requirements. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined before the offset */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        ++last_k;                       /* next candidate k */

        seq_t N = make_N(last_k);
        if (is_prime_seq(N)) {
            /* Grow the cache if necessary. */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {              /* allocation failed – give up */
                    return (seq_t)0;
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)last_k;
        }
    }

    return terms[(size_t)(n - 1)];
}
