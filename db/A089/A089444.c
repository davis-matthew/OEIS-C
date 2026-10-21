/*
 * A089444  Smallest 6-tuple of consecutive primes such that
 *          a(n)+a(n+1)+a(n+2), n=1...4, are again consecutive primes.
 * https://oeis.org/A089444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth prime that belongs to a 6‑tuple of consecutive
 *            primes (p1 ,… ,p6) such that the sums p1+p2+p3, p2+p3+p4,
 *            p3+p4+p5, p4+p5+p6 are themselves four consecutive primes
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches for such tuples by sieving in chunks,
   stores the primes that belong to accepted tuples, and
   returns the nth element of that list. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t) */
#include "../utils/build_primes_upto.h"     /* build_primes_upto() */

static uint64_t *small_primes = NULL;      /* primes up to limit for sieving */
static size_t   small_prime_cnt = 0;

static seq_t  *result   = NULL;            /* accumulated list of primes */
static size_t  result_cap = 0;
static size_t  result_len = 0;             /* number of stored primes */

static uint8_t *segment = NULL;            /* segment bit‑array */
#define SEGMENT_SIZE (1 << 20)             /* 1 048 576 numbers per segment */

/* initialise small primes once */
static void init_small_primes(void)
{
    if (small_primes != NULL)
        return;

    idx_t limit = 1000000;                 /* primes up to 1 000 000 */
    uint64_t *p = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &p, &cnt);    /* provided utility */
    small_primes = p;
    small_prime_cnt = cnt;
}

/* append a prime to the result list, growing the array as needed */
static void append_result(uint64_t v)
{
    if (result_len == result_cap) {
        size_t new_cap = (result_cap == 0) ? 16 : result_cap * 2;
        seq_t *tmp = (seq_t *)realloc(result, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* OOM */
        result = tmp;
        result_cap = new_cap;
    }
    result[result_len++] = (seq_t)v;
}

/* check whether the six primes in w form a required 6‑tuple */
static bool tuple_candidate(const uint64_t *w)
{
    seq_t s[4];
    s[0] = (seq_t)w[0] + (seq_t)w[1] + (seq_t)w[2];
    s[1] = (seq_t)w[1] + (seq_t)w[2] + (seq_t)w[3];
    s[2] = (seq_t)w[2] + (seq_t)w[3] + (seq_t)w[4];
    s[3] = (seq_t)w[3] + (seq_t)w[4] + (seq_t)w[5];

    /* all sums must be prime */
    for (int i = 0; i < 4; ++i)
        if (!is_prime_big(s[i]))
            return false;

    /* the sums must be consecutive primes – no other prime in between */
    for (int i = 0; i < 3; ++i) {
        seq_t lo = s[i];
        seq_t hi = s[i + 1];
        for (seq_t x = lo + 1; x < hi; ++x)
            if (is_prime_big(x))
                return false;
    }
    return true;
}

/* ensure that at least 'need' terms of the sequence are found */
static void ensure_found(idx_t need)
{
    if (result_len >= (size_t)need)
        return;                      /* already have enough */

    init_small_primes();            /* primes for sieving ready */

    if (segment == NULL)
        segment = (uint8_t *)calloc(SEGMENT_SIZE, 1);  /* allocate once */

    uint64_t window[6];
    size_t win_len = 0;

    uint64_t low = 2;                /* start of the first segment */

    while (result_len < (size_t)need) {
        uint64_t high = low + SEGMENT_SIZE - 1;
        if (high < low)              /* overflow */
            break;

        /* clear segment bitmap */
        memset(segment, 0, SEGMENT_SIZE);

        /* sieve composites in [low, high] using small primes */
        for (size_t i = 0; i < small_prime_cnt; ++i) {
            uint64_t p = small_primes[i];
            uint64_t start = (low + p - 1) / p * p;
            if (start < p * 2)
                start = p * 2;
            for (uint64_t x = start; x <= high; x += p)
                segment[x - low] = 1;
        }

        /* scan primes in the segment */
        for (size_t i = 0; i < SEGMENT_SIZE && result_len < (size_t)need; ++i) {
            uint64_t num = low + i;
            if (num < 2 || segment[i])
                continue;               /* composite */

            /* maintain sliding window of six consecutive primes */
            if (win_len < 6) {
                window[win_len++] = num;
            } else {
                memmove(window, window + 1, 5 * sizeof(uint64_t));
                window[5] = num;
            }

            /* test window once it has six primes */
            if (win_len == 6 && tuple_candidate(window)) {
                for (int j = 0; j < 6; ++j)
                    append_result(window[j]);
            }
        }

        low = high + 1;               /* next segment */
    }
}

/* public entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined at 0, return 0 safely */

    ensure_found(n);               /* fill cache up to n terms */
    return result[n - 1];          /* 1‑based indexing */
}
