/*
 * A091318  Lengths of runs of 1's in A039702.
 * https://oeis.org/A091318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091318
   Lengths of consecutive primes congruent 1 mod 4, separated by primes
   congruent 3 mod 4.  No closed formula is known; the algorithm
   enumerates primes in order and records run lengths.  The calculation
   keeps a small static cache of previously computed runs, so calls can
   come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

/* static cache of run lengths */
static seq_t *runs      = NULL;
static size_t runs_cap  = 0;   /* capacity of the cache */
static size_t runs_cnt  = 0;   /* number of runs stored */

/* current state of the prime generator and the unfinished run */
static bool      init        = false;   /* true after first use */
static idx_t     prime_x     = 3;       /* next odd candidate for prime */
static size_t    cur_len     = 0;       /* length of unfinished run */

/* add a new run length to the cache, resizing if necessary */
static void add_run(size_t len)
{
    if (runs_cnt >= runs_cap) {
        size_t new_cap = runs_cap ? runs_cap * 2 : 16;
        void *tmp = realloc(runs, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure; abort */
        runs = (seq_t *)tmp;
        runs_cap = new_cap;
    }
    runs[runs_cnt++] = (seq_t)len;
}

/* fetch the next prime ≥ prime_x; advance prime_x to the next odd number */
static idx_t next_prime(void)
{
    idx_t p = prime_x;
    while (!is_prime(p)) {
        p += 2;               /* skip to next odd number */
    }
    prime_x = p + 2;          /* next candidate after this prime */
    return p;
}

/* generate runs until we have at least 'target' run lengths stored */
static void ensure_runs(idx_t target)
{
    if (runs_cnt >= target) return;

    if (!init) {
        /* start the prime generator at 3 (prime 2 has no effect on runs) */
        prime_x = 3;
        cur_len = 0;
        init = true;
    }

    while (runs_cnt < target) {
        idx_t p = next_prime();
        if ((p & 3u) == 1u) {
            /* prime ≡ 1 (mod 4) – extend current run */
            cur_len++;
        } else {
            /* prime ≡ 3 (mod 4) or other – end current run if any */
            if (cur_len > 0) {
                add_run(cur_len);
                cur_len = 0;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined – but prevents UB */
    ensure_runs(n);
    return runs[n - 1];
}
