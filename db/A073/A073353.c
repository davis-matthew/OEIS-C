/*
 * A073353  Sum of n and its squarefree kernel.
 * https://oeis.org/A073353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n + A007947(n) (squarefree kernel of n)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for incremental sieve */
static seq_t *rad = NULL;      /* rad[i] = product of distinct primes dividing i */
static idx_t rad_size = 0;     /* size of rad array (elements 0..rad_size-1) */
static uint32_t *spf = NULL;   /* lowest prime factor of i */
static idx_t spf_size = 0;     /* size of spf array */

/* extend precomputation up to n (inclusive) */
static void ensure_precompute(idx_t n)
{
    if (rad_size > n) return;               /* already computed */

    /* choose a new size, at least double the current one or at least n+1 */
    idx_t newsize = rad_size > 1 ? rad_size << 1 : 3;
    while (newsize <= n) newsize <<= 1;

    /* allocate new arrays */
    seq_t *newrad = (seq_t*)malloc((newsize + 1) * sizeof(seq_t));
    uint32_t *newspf = (uint32_t*)malloc((newsize + 1) * sizeof(uint32_t));
    if (!newrad || !newspf) {                /* allocation failed – keep old state */
        free(newrad); free(newspf);
        return;
    }

    /* initialise spf array to 0 */
    for (idx_t i = 0; i <= newsize; ++i) newspf[i] = 0;

    /* simple sieve to compute lowest prime factors */
    for (idx_t i = 2; i <= newsize; ++i) {
        if (newspf[i] == 0) {                 /* i is prime */
            newspf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= newsize) {
                for (idx_t j = i * i; j <= newsize; j += i)
                    if (newspf[j] == 0) newspf[j] = (uint32_t)i;
            }
        }
    }

    /* compute rad values incrementally */
    newrad[0] = 0;   /* unused */
    newrad[1] = 1;   /* rad(1)=1 */
    for (idx_t i = 2; i <= newsize; ++i) {
        uint32_t p = newspf[i];
        idx_t m = i / p;
        if (m % p == 0) {
            newrad[i] = newrad[m];            /* p already divides m */
        } else {
            newrad[i] = newrad[m] * (seq_t)p; /* new prime factor p */
        }
    }

    /* replace old arrays */
    free(rad);
    free(spf);
    rad = newrad;
    spf = newspf;
    rad_size = newsize + 1;
    spf_size = rad_size;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_precompute(n);
    return (seq_t)n + rad[n];
}
