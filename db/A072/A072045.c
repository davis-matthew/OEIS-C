/*
 * A072045  Denominator of Product{prime(k)^2/(prime(k)^2 - 1) | 0<k<=n},
 *          Numerator: A072044.
 * https://oeis.org/A072045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A072045:  Denominator of 
 *   𝐹(n) = ∏_{prime(k)^2/(prime(k)^2‑1) | 0<k≤n}
 *  (offset 0:  a(0)=1)
 * The function returns a(n) for any n≥0.
 * It uses prime generation from utils/build_primes.h and gcd from
 * utils/gcd.h.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **out, size_t *cnt) */
#include "../utils/gcd.h"            /* gcd(seq_t a, seq_t b) */

/* ---------- prime storage -------------------------------------------- */
static idx_t *prime_list = NULL;     /* array of primes */
static size_t prime_count = 0;       /* number of primes stored */
static idx_t sieve_limit   = 0;      /* current sieve limit */

/* Ensure at least `need` primes are available (need is an index count). */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;                       /* already have enough */

    /* Choose a new sieve limit that will generate many more primes. */
    if (sieve_limit == 0)
        sieve_limit = 200000;          /* initial limit */
    else
        sieve_limit *= 2;              /* double until enough */

    while (prime_count < need) {
        idx_t *new_list = NULL;
        size_t new_cnt = 0;
        build_primes(sieve_limit, (idx_t **)&new_list, &new_cnt);
        if (prime_list)
            free(prime_list);          /* discard old list */
        prime_list  = new_list;
        prime_count = new_cnt;
        if (prime_count >= need)
            break;
        sieve_limit *= 2;              /* increase limit and retry */
    }
}

/* ---------- caching denominators -------------------------------------- */
static seq_t *denoms      = NULL;    /* denoms[i] = a(i) */
static size_t denom_rows  = 0;       /* allocated size */

/* Grow the denominator cache if `need` entries are required. */
static void grow_denoms(size_t need)
{
    if (denom_rows >= need)
        return;
    size_t new_size = (need + 10) * 2;
    seq_t *tmp = (seq_t *)realloc(denoms, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    denoms = tmp;
    denom_rows = new_size;
}

/* ---------- product state ---------------------------------------------- */
static seq_t prod_num = 1;           /* numerator of the running product */
static seq_t prod_den = 1;           /* denominator of the running product */
static size_t prod_index = 0;        /* next index to compute (1‑based); 0 means none */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (!denoms) {
        grow_denoms((size_t)n + 10);
        denoms[0] = 1;                /* a(0) = 1 */
        prod_num = 1;
        prod_den = 1;
        prod_index = 1;               /* Next index to compute is 1. */
    }

    /* Ensure we have at least n primes (primes are 0‑based). */
    ensure_primes((size_t)n);

    /* Compute successive terms up to n if they haven't been cached. */
    while (prod_index <= (size_t)n) {
        idx_t k = prod_index - 1;          /* prime index (0‑based) */
        idx_t p = prime_list[k];           /* prime value */

        seq_t num = (seq_t)p * (seq_t)p;    /* p² */
        seq_t den = (seq_t)p * (seq_t)p - 1;/* p² – 1 */

        /* Cancel factors between num and current denominator. */
        seq_t g = gcd(num, prod_den);
        num   /= g;
        prod_den /= g;

        /* Cancel factors between den and current numerator. */
        g = gcd(den, prod_num);
        den   /= g;
        prod_num /= g;

        /* Update product. */
        prod_num *= num;
        prod_den *= den;

        /* Store the denominator for this index. */
        denoms[prod_index] = prod_den;
        prod_index++;
    }

    return denoms[n];
}
