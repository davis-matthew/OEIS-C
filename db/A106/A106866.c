/*
 * A106866  Primes of the form 2x^2+xy+3y^2, with x and y nonnegative.
 * https://oeis.org/A106866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 2x² + x·y + 3y² with x,y ≥ 0
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   cmp_seq_t, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/isPrime64.h"
#include "../utils/cmp_seq_t.h"

static seq_t *primes = NULL;      /* array of found primes */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t limit = 1000;       /* current sieve limit */

/* Helper: generate all primes of the required form up to value L */
static void generate_primes_up_to(size_t L)
{
    /* bit array of seen values */
    unsigned char *seen = (unsigned char*)calloc(L + 1, 1);
    if (!seen) exit(EXIT_FAILURE);

    /* temporary storage for primes */
    size_t local_cap = 1024;
    seq_t *local = (seq_t*)malloc(local_cap * sizeof(seq_t));
    if (!local) { free(seen); exit(EXIT_FAILURE); }
    size_t local_cnt = 0;

    /* integer sqrt helper */
    double sqrtL = sqrt((double)L);
    uint64_t y_max = (uint64_t)(sqrtL / sqrt(3.0)) + 1;

    for (uint64_t y = 0; y <= y_max; ++y) {
        uint64_t y3 = 3 * y * y;
        if (y3 > L) break;
        for (uint64_t x = 0; ; ++x) {
            uint64_t val = 2 * x * x + x * y + y3;
            if (val > L) break;
            size_t iv = (size_t)val;
            if (!seen[iv]) {
                seen[iv] = 1;
                if (isPrime64((uint64_t)val)) {
                    if (local_cnt == local_cap) {
                        local_cap *= 2;
                        local = (seq_t*)realloc(local, local_cap * sizeof(seq_t));
                        if (!local) { free(seen); exit(EXIT_FAILURE); }
                    }
                    local[local_cnt++] = (seq_t)val;
                }
            }
        }
    }

    /* sort the local list */
    qsort(local, local_cnt, sizeof(seq_t), cmp_seq_t);

    /* replace the global table */
    free(primes);
    primes = local;
    primes_cnt = local_cnt;

    free(seen);
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard against wrap‑around */

    /* lazy initialisation and extension */
    if (!primes) {
        generate_primes_up_to(limit);
    }
    while (primes_cnt < (size_t)n) {
        limit <<= 1;  /* double the limit and regenerate */
        generate_primes_up_to(limit);
    }

    return primes[n - 1];
}
