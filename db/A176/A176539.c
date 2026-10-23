/*
 * A176539  Indices of nonprime numbers in the products of two noncomposite
 *          numbers.
 * https://oeis.org/A176539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   indices of nonprime numbers in A037143 (numbers that are
 *            products of two noncomposite numbers). a(n) is the position in
 *            A037143 of the n‑th nonprime member (1, 4, 6, 8, 9, …)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of the helper that rebuilds the data up to a new bound */
static void recompute(idx_t new_limit);

/* Global static storage for the data */
static idx_t cur_limit = 0;    /* current upper bound that has been sieved */
static seq_t *indices   = NULL;/* array holding the indices of nonprime terms */
static size_t indices_n = 0;   /* number of indices stored */

/* Recompute the sequence of positions of nonprime numbers in A037143
   up to the new bound `new_limit`.  The arrays are freshly allocated,
   so any previously stored data is discarded. */
static void recompute(idx_t new_limit)
{
    /* Allocate/resize the array that will hold the indices. */
    if (indices) {
        free(indices);
    }
    indices = (seq_t*)malloc(new_limit * sizeof(seq_t));
    if (!indices) {                 /* allocation failure – nothing to do */
        indices_n = 0;
        return;
    }

    /* Linear sieve for smallest prime factor up to new_limit. */
    uint32_t *spf = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    idx_t *primes = (idx_t*)malloc(new_limit * sizeof(idx_t));
    if (!spf || !primes) {
        free(indices);
        indices = NULL;
        indices_n = 0;
        free(spf); free(primes);
        return;
    }

    for (idx_t i = 0; i <= new_limit; ++i) {
        spf[i] = 0;
    }

    size_t pcnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > new_limit) {
                break;
            }
            if (p > spf[i]) {
                break;
            }
            spf[ip] = (uint32_t)p;
        }
    }
    free(primes);

    /* Scan the numbers 1 … new_limit, counting positions in A037143,
       and store the positions that correspond to nonprime members. */
    idx_t pos = 0;            /* position in A037143 */
    size_t out = 0;           /* number of indices collected so far */

    for (idx_t m = 1; m <= new_limit; ++m) {
        int omega = 0;
        if (m == 1) {
            omega = 0;
        } else {
            idx_t x = m;
            while (x > 1 && omega <= 2) {
                ++omega;
                x /= spf[x];
            }
        }

        if (omega <= 2) {      /* m belongs to A037143 */
            ++pos;
            if (omega != 1) {  /* nonprime: 1 or composite with ≤2 prime factors */
                if (out < new_limit) {
                    indices[out++] = (seq_t)pos;
                }
            }
        }
    }

    indices_n = out;
    free(spf);
    cur_limit = new_limit;
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;            /* undefined, but safe guard */
    }

    if (indices == NULL) {
        /* Initial bound — chosen so that the first few terms are produced
           without requiring many resizes; a reasonably small value works. */
        recompute(50000);
    }

    /* Grow the bound until we have enough indices. */
    while (indices_n < n) {
        idx_t new_limit = cur_limit ? cur_limit * 2 : 50000;
        recompute(new_limit);
    }

    return indices[n - 1];
}
