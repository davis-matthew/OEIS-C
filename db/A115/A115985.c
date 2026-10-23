/*
 * A115985  Positive n which are not the average of a run of at least 2
 *          consecutive primes.
 * https://oeis.org/A115985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: numbers that are NOT the average of a run of at least two consecutive primes.
   The sequence is built by sieving primes up to 2⋅limit+100, marking every integer that can
   be expressed as an average of a consecutive‑prime block, and collecting the unmarked
   integers.  The resulting list is stored in a static array and is expanded by
   repeated doubling of the limit until the requested term is available. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* generate all primes ≤ limit and return them in *primes (malloc'ed) and their count *cnt */
static void generate_primes_upto(idx_t limit, idx_t **primes, size_t *cnt)
{
    if (limit < 2) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    bool *is_prime = (bool*)malloc((size_t)(limit + 1));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++c;
    idx_t *p = (idx_t*)malloc(c * sizeof(idx_t));
    if (!p) { free(is_prime); exit(1); }
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) p[k++] = i;
    *primes = p;
    *cnt = c;
    free(is_prime);
}

static void recompute(idx_t new_limit,
                      idx_t **primes, size_t *prCnt,
                      bool **isAvg, size_t *isAvgSize,
                      seq_t **vals, size_t *valsCnt)
{
    /* free previous data */
    if (*primes) free(*primes);
    if (*isAvg)  free(*isAvg);
    if (*vals)   free(*vals);

    /* generate primes up to a safety margin (enough for all averages ≤ new_limit) */
    idx_t prime_limit = new_limit * 2 + 100;
    generate_primes_upto(prime_limit, primes, prCnt);

    /* array marking integers that are averages */
    *isAvgSize = (size_t)(new_limit + 1);
    bool *avg_mark = (bool*)malloc(*isAvgSize);
    if (!avg_mark) exit(1);
    for (size_t i = 0; i < *isAvgSize; ++i) avg_mark[i] = false;

    /* mark averages of consecutive primes */
    for (size_t si = 0; si + 1 < *prCnt; ++si) {
        unsigned __int128 sum = (*primes)[si];
        for (size_t ei = si + 1; ei < *prCnt; ++ei) {
            sum += (*primes)[ei];
            size_t len = ei - si + 1;
            if (sum % len == 0) {
                unsigned __int128 avg128 = sum / len;
                if (avg128 <= (unsigned __int128)new_limit) {
                    size_t avg = (size_t)avg128;
                    avg_mark[avg] = true;
                }
            }
        }
    }
    *isAvg = avg_mark;

    /* build the list of non‑average integers */
    size_t capacity = (size_t)new_limit;        /* enough for numbers ≤ new_limit */
    seq_t *list = (seq_t*)malloc(capacity * sizeof(seq_t));
    if (!list) exit(1);
    size_t ncnt = 0;
    for (size_t v = 1; v <= (size_t)new_limit; ++v) {
        if (!avg_mark[v]) {
            list[ncnt++] = (seq_t)v;
            if (ncnt == capacity) {
                capacity <<= 1;
                list = (seq_t*)realloc(list, capacity * sizeof(seq_t));
                if (!list) exit(1);
            }
        }
    }
    *vals = list;
    *valsCnt = ncnt;
}

seq_t a(idx_t n)
{
    /* static state */
    static idx_t limit = 1000;              /* initial bound */
    static idx_t *primes = NULL;
    static size_t primesCnt = 0;
    static bool *isAvg = NULL;
    static size_t isAvgSize = 0;
    static seq_t *vals = NULL;
    static size_t valsCnt = 0;

    /* first call → build initial data */
    if (vals == NULL) {
        recompute(limit, &primes, &primesCnt, &isAvg, &isAvgSize, &vals, &valsCnt);
    }

    /* grow the limit until we have at least n terms */
    while (valsCnt < n) {
        limit <<= 1;   /* double the bound */
        recompute(limit, &primes, &primesCnt, &isAvg, &isAvgSize, &vals, &valsCnt);
    }
    return vals[n - 1];
}
