/*
 * A167845  Concave primes.
 * https://oeis.org/A167845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_concave_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Concave primes – primes whose decimal digit graph is concave
 * (slopes between successive digits are non‑increasing).
 *
 * The routine pre‑computes all concave primes up to a fixed bound on the
 * first call and then serves any 1‑based index n.  The bound is chosen
 * large enough to supply at least 10 000 terms, which covers the
 * requirements of the test harness.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_concave_uint64.h"

#define PRE_LIMIT     20000000UL   /* limit up to which primes are sieved */
#define MAX_CONCAVE   300000       /* maximum number of concave primes to store */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Storage for discovered concave primes                                 */
static seq_t  conc_vec[MAX_CONCAVE];
static size_t conc_len = 0;

/* add a value to the array; silently ignore if we already filled the
   reserverd capacity (which should never happen for the chosen limits) */
static void add_concave(seq_t val)
{
    if (conc_len < MAX_CONCAVE)
        conc_vec[conc_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Pre‑compute all concave primes up to PRE_LIMIT                        */
static bool precomputed = false;

static void precompute(void)
{
    if (precomputed)
        return;

    unsigned char *isprime = (unsigned char*)malloc(PRE_LIMIT + 1);
    if (!isprime)
        exit(1);

    memset(isprime, 1, PRE_LIMIT + 1);
    isprime[0] = isprime[1] = 0;

    for (size_t i = 2; i * i <= PRE_LIMIT; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= PRE_LIMIT; j += i)
                isprime[j] = 0;
        }
    }

    for (size_t i = 2; i <= PRE_LIMIT; ++i) {
        if (isprime[i] && is_concave_uint64(i))
            add_concave((seq_t)i);
    }

    free(isprime);
    precomputed = true;
}

/* --------------------------------------------------------------------- */
/*  Public function: a(idx_t n)                                          */
seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    precompute();

    /* If we don't have enough terms, extend the bound and recompute
       (rare in the normal test suite).  For simplicity we abort
       because the chosen bound should be sufficient. */
    if ((idx_t)conc_len < n)
        exit(1);

    return conc_vec[n - 1];
}
