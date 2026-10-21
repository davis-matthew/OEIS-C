/*
 * A097641  Numbers n such that n=d_1^pi(d_1)+d_2^pi(d_2)+...+d_k^pi(d_k)
 *          where d_1 d_2 ... d_k is the decimal expansion of n.
 * https://oeis.org/A097641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   seq_alloc, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes OEIS sequence A097641:
 * numbers n that satisfy n = Σ d_i^π(d_i) over the decimal digits d_i of n. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_pow.h"

static const int pi_table[10] = {0,0,1,2,2,3,3,4,4,4};

static seq_t *terms = NULL;      /* array of sequence terms */
static idx_t  terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* current capacity of terms array */

#define MAX_N 40000L   /* upper bound necessary to find all terms */

/* Initialise the sequence once */
static void init_sequence(void)
{
    size_t cap = 64;
    terms = (seq_t*)seq_alloc(cap);
    terms_cap = cap;

    for (idx_t n = 1; n <= MAX_N; ++n) {
        seq_t sum = 0;
        idx_t x = n;
        while (x > 0) {
            int d = x % 10;
            int p = pi_table[d];
            seq_t t = seq_pow((seq_t)d, (idx_t)p);
            sum += t;
            x /= 10;
        }
        if (sum == (seq_t)n) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                terms = (seq_t*)seq_realloc(terms, terms_cap, new_cap);
                terms_cap = new_cap;
            }
            terms[terms_len++] = sum;
        }
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (!terms) init_sequence();
    if (n == 0 || n > terms_len) return 0;
    return terms[n - 1];
}
