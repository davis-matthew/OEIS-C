/*
 * A067140  Primes p beginning consecutive prime-difference pattern as
 *          follows: p, (10, 2, 10, 2, 10), p+34.
 * https://oeis.org/A067140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p, p+10, p+12, p+22, p+24,
 *            p+34 are consecutive primes
 * Range:     u8 none, u16 n <= 2, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Small prime sieve for the segmented sieve                     */
/* ------------------------------------------------------------ */
#define SMALL_PRIME_LIMIT 1000000 /* enough for segments up to ~1e12 */

static uint32_t *small_primes = NULL;      /* array of small primes   */
static size_t    small_prime_count = 0;    /* number of small primes */
static bool      small_primes_init  = false;

static void init_small_primes(void) {
    if (small_primes_init) return;

    size_t limit = SMALL_PRIME_LIMIT;
    bool *mark = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!mark) return; /* allocation failure – unlikely */

    for (size_t i = 2; i <= limit; ++i) mark[i] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j <= limit; j += i) mark[j] = false;
        }
    }
    /* Count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (mark[i]) small_prime_count++;

    /* Allocate and store primes */
    small_primes = (uint32_t*)malloc(small_prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (mark[i]) small_primes[idx++] = (uint32_t)i;

    free(mark);
    small_primes_init = true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
/* ------------------------------------------------------------ */
#define SEGMENT_SIZE (1 << 20) /* 1 MiB segment for sieving */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; offset is 1 */

    /* Static state for memoisation */
    static seq_t *results = NULL;     /* array of found primes  */
    static size_t res_len = 0;        /* number of results stored */
    static size_t res_cap = 0;        /* allocation capacity     */
    static seq_t ring[6];             /* last 6 primes encountered */
    static int ring_used = 0;         /* how many primes currently in ring */
    static idx_t segment_low = 2;     /* start of next segment to sieve */

    /* Ensure small primes for sieving */
    if (!small_primes_init) init_small_primes();
    if (!small_primes) return 0; /* allocation failure */

    /* Extend array lazily */
    if (!results) {
        res_cap = 64;
        results = (seq_t*)malloc(res_cap * sizeof(seq_t));
        if (!results) return 0;
    }

    /* If already have enough results, return */
    if ((size_t)n <= res_len) return results[n - 1];

    /* Scan until we accumulate n results */
    while (res_len < (size_t)n) {
        idx_t segment_high = segment_low + SEGMENT_SIZE;
        size_t segsize = (size_t)(segment_high - segment_low);

        char *segment = (char*)malloc(segsize);
        if (!segment) return 0; /* allocation failure */

        memset(segment, 1, segsize); /* assume all are prime initially */

        /* Sieve the segment with the small primes */
        for (size_t i = 0; i < small_prime_count; ++i) {
            uint32_t p = small_primes[i];
            idx_t start = (segment_low + p - 1) / p * p;
            idx_t p_sq  = (idx_t)p * (idx_t)p;
            if ((idx_t)start < p_sq) start = p_sq;
            if ((idx_t)start >= segment_high) continue;

            for (idx_t j = start; j < segment_high; j += p)
                segment[j - segment_low] = 0;
        }

        /* Process the primes in this segment in order */
        for (idx_t x = segment_low; x < segment_high; ++x) {
            if (x < 2) continue;                /* 0 and 1 are not prime */
            if (!segment[x - segment_low]) continue; /* composite */

            /* x is a prime */
            if (ring_used < 6) {
                ring[ring_used++] = (seq_t)x;
            } else {
                /* Test the current window of 6 consecutive primes */
                if (ring[1] - ring[0] == 10 &&
                    ring[2] - ring[1] == 2 &&
                    ring[3] - ring[2] == 10 &&
                    ring[4] - ring[3] == 2 &&
                    ring[5] - ring[4] == 10)
                {
                    if (res_len == res_cap) {
                        res_cap = (res_cap == 0) ? 64 : res_cap << 1;
                        seq_t *tmp = (seq_t*)realloc(results, res_cap * sizeof(seq_t));
                        if (!tmp) { free(segment); return 0; }
                        results = tmp;
                    }
                    results[res_len++] = ring[0];
                }

                /* Shift the ring left and add the new prime */
                for (int i = 0; i < 5; ++i) ring[i] = ring[i + 1];
                ring[5] = (seq_t)x;
            }
        }

        free(segment);
        segment_low = segment_high; /* move to next segment */
    }

    /* We now have at least n results */
    return results[n - 1];
}
