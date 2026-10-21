/*
 * A064820  a(n) = Product_{k=1..4*n-9} prime(k) - prime(4n)^4 where prime(i)
 *          = i-th prime.
 * https://oeis.org/A064820
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{k=1}^{4*n-9} prime(k) – prime(4*n)^4
 * Range:     i8 none, i16 none, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* dynamically stored primes and prefix products */
static uint32_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, ... */
static size_t    primes_cnt = 0;     /* number of primes currently stored */

static seq_t *prefix = NULL;         /* prefix[i] = product of first i primes, prefix[0] = 1 */
static size_t  prefix_len = 0;       /* = primes_cnt + 1 */

/* expand the prime list and prefix products to contain at least 'need' primes */
static void ensure_primes(idx_t need)
{
    if (need <= primes_cnt)
        return;                      /* already have enough primes */

    size_t new_cnt = (size_t)need;

    /* Allocate / grow the prime array */
    if (primes == NULL) {
        primes = (uint32_t *)malloc(new_cnt * sizeof(uint32_t));
        primes_cnt = 0;
    } else {
        primes = (uint32_t *)realloc(primes, new_cnt * sizeof(uint32_t));
    }

    /* Initially, if the list is empty, put the first prime 2 */
    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* Generate the missing primes */
    for (uint32_t cand = (primes_cnt == 1 ? 3 : primes[primes_cnt - 1] + 2);
         primes_cnt < new_cnt; cand += 2) {
        int is_prime = 1;
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            primes[primes_cnt++] = cand;
    }

    /* Rebuild / extend the prefix product array */
    size_t new_prefix_len = primes_cnt + 1;
    seq_t *new_prefix = NULL;

    if (prefix == NULL) {
        new_prefix = (seq_t *)malloc(new_prefix_len * sizeof(seq_t));
        new_prefix[0] = (seq_t)1;
        for (size_t i = 1; i <= primes_cnt; ++i)
            new_prefix[i] = new_prefix[i - 1] * (seq_t)primes[i - 1];
        prefix = new_prefix;
        prefix_len = new_prefix_len;
    } else {
        new_prefix = (seq_t *)realloc(prefix, new_prefix_len * sizeof(seq_t));
        /* compute only the new entries */
        for (size_t i = prefix_len; i <= primes_cnt; ++i)
            new_prefix[i] = new_prefix[i - 1] * (seq_t)primes[i - 1];
        prefix = new_prefix;
        prefix_len = new_prefix_len;
    }
}

/* Sequence A064820 ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3)                     /* undefined for n < 3, return 0 */
        return (seq_t)0;

    idx_t primes_needed = 4 * n;    /* need at least the 4*n‑th prime */
    ensure_primes(primes_needed);

    idx_t prod_idx = 4 * n - 9;     /* product of first 4*n‑9 primes */
    seq_t prod = prefix[prod_idx];

    uint32_t p4 = primes[4 * n - 1];  /* 4*n‑th prime (0‑based index) */

    seq_t p = (seq_t)p4;
    seq_t p2 = p * p;
    seq_t p4pow = p2 * p2;          /* prime(4*n)^4 */

    return prod - p4pow;
}
