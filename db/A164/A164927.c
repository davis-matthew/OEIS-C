/*
 * A164927  Sum of the odd prime divisors of numbers with all odd prime
 *          divisors of the form 4k+1.
 * https://oeis.org/A164927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the sum of the distinct odd prime factors of the * n‑th
 *            smallest integer whose odd prime factors all belong to * {p : p
 *            ≡ 1 (mod 4)} and contains at least one such prime. * The
 *            sequence is obtained by generating the “humble” numbers whose *
 *            base primes are 2 and the primes ≡ 1 (mod 4); fo...
 * Range:     u8 none, u16 none, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static bool initialized = false;

/* list of allowed prime bases: 2 followed by all odd primes ≡ 1 (mod 4) */
static uint64_t *prime_list = NULL;
static size_t prime_len = 0;          /* number of primes in prime_list */

/* index pointer for each prime in the humble generation algorithm */
static size_t *prime_idx_ptr = NULL;

/* humble numbers array (sorted ascending).  We store 1 initially. */
static seq_t *humble = NULL;
static size_t humble_len = 0;
static size_t humble_cap = 0;

/* cache of a(n) values (0‑based). */
static seq_t *a_cache = NULL;
static size_t a_len = 0;
static size_t a_cap = 0;

/* ------------------------------------------------------------------ */
/* one‑time initialisation                                             */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    /* Build a list of ordinary primes up to a generous limit; later
     * we filter those ≡ 1 (mod 4).  The limit chosen guarantees that
     * with at most 10000 terms the humble generator will never request
     * a prime beyond it. */
    uint64_t *primes_tmp = NULL;
    size_t primes_tmp_cnt = 0;
    /* 200 000 is more than enough for the first 10 000 terms. */
    build_primes_upto(200000, &primes_tmp, &primes_tmp_cnt);

    /* Count how many of them are ≡ 1 (mod 4) */
    size_t odd_cnt = 0;
    for (size_t i = 0; i < primes_tmp_cnt; ++i)
        if (primes_tmp[i] % 4ull == 1ull)
            ++odd_cnt;

    /* Allocate space for prime_list (2 + odd_cnt) */
    prime_len = odd_cnt + 1;
    prime_list = (uint64_t *)malloc(prime_len * sizeof(uint64_t));
    prime_list[0] = 2ull;              /* include 2 as a base prime */

    size_t j = 1;
    for (size_t i = 0; i < primes_tmp_cnt; ++i)
        if (primes_tmp[i] % 4ull == 1ull)
            prime_list[j++] = primes_tmp[i];
    free(primes_tmp);

    /* Allocate index pointers for each prime and initialise to 0 */
    prime_idx_ptr = (size_t *)calloc(prime_len, sizeof(size_t));

    /* Humble array: reserve a generous capacity.  300 000 is more than
     * enough for the first 10000 terms.  It keeps the algorithm
     * O(n * |prime_list|). */
    humble_cap = 300000;
    humble = (seq_t *)malloc(humble_cap * sizeof(seq_t));
    humble[0] = 1;
    humble_len = 1;

    /* Cache for a(n).  A capacity of 20000 is safe for up to n=10000. */
    a_cap = 20000;
    a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    a_len = 0;
}

/* ------------------------------------------------------------------ */
/* generate terms until we have at least 'target' entries in a_cache   */
/* ------------------------------------------------------------------ */
static void generate_until(size_t target)
{
    for (;;)
    {
        /* produce the next humble number: min over primes of
         * prime * humble[prime_idx_ptr[prime]] */
        seq_t next;
        bool first = true;
        for (size_t k = 0; k < prime_len; ++k)
        {
            seq_t cand = (seq_t)prime_list[k] * humble[prime_idx_ptr[k]];
            if (first || cand < next)
            {
                next = cand;
                first = false;
            }
        }

        /* append next to the humble array */
        if (humble_len == humble_cap)
        {
            /* unlikely to happen with the chosen capacities */
            humble_cap <<= 1;
            humble = (seq_t *)realloc(humble, humble_cap * sizeof(seq_t));
        }
        humble[humble_len++] = next;

        /* increment indices that produced this minimum */
        for (size_t k = 0; k < prime_len; ++k)
        {
            seq_t cand = (seq_t)prime_list[k] * humble[prime_idx_ptr[k]];
            if (cand == next)
                ++prime_idx_ptr[k];
        }

        /* skip 1 and pure powers of 2 */
        if (next == (seq_t)1)
            continue;
        if ((next & (next - (seq_t)1)) == (seq_t)0)   /* power of two */
            continue;

        /* compute sum of distinct odd prime factors */
        uint64_t odd = (uint64_t)next;
        while ((odd & 1ull) == 0)
            odd >>= 1;                      /* strip powers of two */

        uint64_t pf[64];
        uint64_t ex[64];
        size_t pf_cnt = 0;
        factor_u64(odd, pf, ex, &pf_cnt);

        seq_t sum = 0;
        for (size_t i = 0; i < pf_cnt; ++i)
            sum += (seq_t)pf[i];

        /* store the result in the cache */
        if (a_len == a_cap)
        {
            a_cap <<= 1;
            a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        }
        a_cache[a_len++] = sum;

        if (a_len >= target)
            break;
    }
}

/* ------------------------------------------------------------------ */
/* public function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (!initialized)
    {
        init_sequence();
        initialized = true;
    }

    size_t target = (size_t)n;
    if (a_len < target)
        generate_until(target);

    return a_cache[n - 1];
}
