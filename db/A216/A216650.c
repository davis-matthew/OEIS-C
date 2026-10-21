/*
 * A216650  Maximum length of each subsequence whose elements are the greatest
 *          prime divisors of the integers 2, 3, 4, ... in increasing order.
 * https://oeis.org/A216650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum length of consecutive integers starting at
 *            some m so that the greatest prime factors are strictly
 *            increasing; a(n) is the length of the nth such block (offset 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* static state */
static idx_t cur_limit = 0;              /* current sieve limit (>=2) */
static uint32_t *maxp = NULL;            /* max prime factor for each integer */
static uint32_t *primes = NULL;          /* list of primes up to cur_limit */
static size_t prime_cnt = 0;             /* number of primes */
static idx_t last_processed = 1;         /* last integer that has been assigned to a block */
static seq_t *block_len = NULL;          /* a(1),a(2),... */
static size_t block_len_cnt = 0;         /* number of computed a(n) values */

/* rebuild sieve and maxp array up to new_limit (>=2) */
static void rebuild_sieve(idx_t new_limit) {
    /* free old data */
    free(maxp);
    free(primes);
    cur_limit = new_limit;

    /* allocate maxp array */
    maxp = (uint32_t*)calloc((size_t)(cur_limit + 1), sizeof(uint32_t));
    if (!maxp) exit(EXIT_FAILURE);

    /* simple Eratosthenes to collect primes */
    bool *is_prime = (bool*)malloc((size_t)(cur_limit + 1));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= cur_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= cur_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= cur_limit; j += i)
                is_prime[j] = false;
        }
    }
    /* count primes to allocate array */
    size_t pc = 0;
    for (idx_t i = 2; i <= cur_limit; ++i) if (is_prime[i]) ++pc;
    primes = (uint32_t*)malloc(pc * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    prime_cnt = 0;
    for (idx_t i = 2; i <= cur_limit; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;
    free(is_prime);

    /* compute max prime factor for each integer */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        for (uint32_t m = p; m <= cur_limit; m += p)
            maxp[m] = p;            /* later primes override earlier ones */
    }
}

/* ensure that the sieve covers at least up to needed_lim (>last_processed) */
static void ensure_limit(idx_t needed_lim) {
    if (needed_lim <= cur_limit) return;
    /* double until >= needed_lim, start from 2 if first time */
    idx_t new_lim = (cur_limit == 0) ? 4 : cur_limit;
    while (new_lim < needed_lim) new_lim <<= 1;
    rebuild_sieve(new_lim);
}

/* compute a(n) for n>=1, building up to this index on demand */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined, but safe */
    /* compute until we have at least n terms */
    while (block_len_cnt < n) {
        /* make sure we have range for next block start */
        ensure_limit(last_processed + 1);

        idx_t start = last_processed + 1;
        idx_t j = start;

        /* find longest increasing run of gpf starting at 'start' */
        while (true) {
            if (j + 1 > cur_limit) {
                /* need more numbers; extend limit and recompute */
                ensure_limit(cur_limit * 2 + 100);
                continue;               /* after extending, retry the loop */
            }
            if (maxp[j + 1] > maxp[j]) {
                ++j;
                continue;
            }
            break;                       /* run ended */
        }

        /* store length of this block */
        seq_t len = (seq_t)(j - start + 1);
        seq_t *tmp = (seq_t*)realloc(block_len, (block_len_cnt + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        block_len = tmp;
        block_len[block_len_cnt++] = len;

        last_processed = j;
    }

    return block_len[n - 1];
}
