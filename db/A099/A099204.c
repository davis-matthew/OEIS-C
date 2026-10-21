/*
 * A099204  A variation on Flavius's sieve (A000960): Start with the natural
 *          numbers; at the k-th sieving step, remove every p-th term of the
 *          sequence remaining after the (k-1)-st sieving step, where p is the
 *          k-th prime; iterate.
 * https://oeis.org/A099204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   by iterative Flavius sieve – remove every p-th term for each
 *            prime p
 * Range:     u8 n <= 40, u16 n <= 4586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* compute the n-th term of A099204 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety guard for offset 1 */

    /* start with a generous bound; it will be doubled if necessary */
    idx_t bound = (n < 10) ? 10 : n * 5;

    for (;;) {
        /* -----------  generate primes up to 'bound'  ----------- */
        size_t sz = (size_t)bound + 1ULL;
        unsigned char *prime_mark = (unsigned char*)calloc(sz, sizeof(unsigned char));
        if (!prime_mark) return (seq_t)0;          /* allocation failure */

        for (size_t i = 2; i * i <= (size_t)bound; ++i) {
            if (!prime_mark[i]) {
                for (size_t j = i * i; j <= (size_t)bound; j += i)
                    prime_mark[j] = 1;
            }
        }

        size_t maxp = bound / 10 + 10;            /* crude estimate for #primes */
        idx_t *primes = (idx_t*)malloc(maxp * sizeof(idx_t));
        if (!primes) { free(prime_mark); return (seq_t)0; }

        size_t pcnt = 0;
        for (size_t i = 2; i <= (size_t)bound; ++i) {
            if (!prime_mark[i]) {
                primes[pcnt++] = (idx_t)i;
                if (pcnt >= maxp) {
                    /* enlarge the prime array if needed */
                    maxp *= 2;
                    idx_t *tmp = (idx_t*)realloc(primes, maxp * sizeof(idx_t));
                    if (!tmp) { free(primes); free(prime_mark); return (seq_t)0; }
                    primes = tmp;
                }
            }
        }
        free(prime_mark);

        /* -----------  build initial list 1 .. bound  ----------- */
        size_t cur_len = (size_t)bound;
        idx_t *cur = (idx_t*)malloc(cur_len * sizeof(idx_t));
        if (!cur) { free(primes); return (seq_t)0; }
        for (size_t i = 0; i < cur_len; ++i) cur[i] = (idx_t)(i + 1);

        /* -----------  perform sieving steps  ----------- */
        for (size_t step = 0; step < pcnt && cur_len > 0; ++step) {
            idx_t p = primes[step];
            if (cur_len < (size_t)p) break;                /* no more deletions */

            size_t next_len = cur_len - cur_len / (size_t)p;
            idx_t *next = (idx_t*)malloc(next_len * sizeof(idx_t));
            if (!next) { free(cur); free(primes); return (seq_t)0; }

            size_t j = 0;
            for (size_t i = 0; i < cur_len; ++i) {
                if (((i + 1) % p) != 0) {
                    next[j++] = cur[i];
                }
            }

            free(cur);
            cur = next;
            cur_len = next_len;
        }
        free(primes);

        /* -----------  check if we have enough survivors  ----------- */
        if (cur_len >= (size_t)n) {
            seq_t result = (seq_t)cur[n - 1];           /* 1‑based indexing */
            free(cur);
            return result;
        }

        free(cur);
        bound *= 2;   /* insufficient, try a larger bound */
    }
}
