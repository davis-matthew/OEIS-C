/*
 * A233399  A "turtle transform" of the primes p=A000040: coordinates
 *          (x,y)=(a(2n),a(2n+1)) of a walk on the complex plane, where at
 *          step n the cursor turns by arg(i^p(n)), then moves p(n) units
 *          ahead.
 * https://oeis.org/A233399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, walk
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A233399: the turtle transform of primes.
   Execution keeps a cache of the walk coordinates.
   The walk proceeds step by step: starting at (0,0) facing east.
   At step k (k>=1) a prime p(k) is taken, the direction is rotated left by
   p(k) * 90° (i.e., Dir = (Dir + p(k) mod 4) mod 4), then the point is moved
   p(k) units in that direction.  The sequence is a(0)=a(1)=0, and for
   k>=1: a(2k)=x after step k, a(2k+1)=y after step k.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static void init_primes(uint32_t **out, size_t *cnt, uint32_t limit)
{
    /* Simple sieve up to limit.  */
    size_t sz = (size_t)limit + 1;
    uint8_t *may = (uint8_t*)calloc(sz, 1);
    if (!may) return;
    for (uint32_t i = 2; i <= limit; ++i) may[i] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (may[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) may[j] = 0;
        }
    }
    size_t num = 0;
    for (uint32_t i = 2; i <= limit; ++i) if (may[i]) num++;
    uint32_t *prs = (uint32_t*)malloc(num * sizeof(uint32_t));
    if (!prs) { free(may); return; }
    for (uint32_t i = 2, j = 0; i <= limit; ++i) if (may[i]) prs[j++] = i;
    free(may);
    *out  = prs;
    *cnt  = num;
}

/* Global cache for primes up to 200000 (enough for 10000 steps). */
static uint32_t *cached_primes = NULL;
static size_t    cached_primes_cnt = 0;

static void ensure_primes(void)
{
    if (cached_primes) return;
    init_primes(&cached_primes, &cached_primes_cnt, 200000U);
}

/* Compute A233399. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n <= 1) return (seq_t)0;

    /* Static cache for walk coordinates: a[0], a[1], a[2], ... */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed_steps = 0;      /* number of steps computed. */
    static seq_t x = 0, y = 0;            /* current coordinates. */
    static int dir = 0;                   /* 0 east, 1 north, 2 west, 3 south. */

    /* Base indices (0,1 are already 0).  */
    if (!cache) {
        cap = 4;          /* enough for indices 0..3 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = cache[1] = (seq_t)0;
    }

    /* Ensure primes are available. */
    ensure_primes();

    /* Determine how many steps are needed to reach a[n]. */
    idx_t step_needed = n / 2;            /* floor(n/2) */
    if (step_needed > computed_steps) {
        /* Allocate more space for the cache as needed. */
        size_t new_cap = (size_t)(2 * step_needed + 2);
        if (new_cap > cap) {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        /* Compute missing steps one by one. */
        for (idx_t k = computed_steps; k < step_needed; ++k) {
            /* k is zero-based step index; prime number p(k) is the (k+1)-th prime. */
            uint32_t p = cached_primes[(size_t)k];
            int turn = (int)(p % 4);        /* 0..3 turns left */
            dir = (dir + turn) & 3;         /* update direction */

            /* Move in the new direction. */
            switch (dir) {
                case 0: x += (seq_t)p;      break; /* east */
                case 1: y += (seq_t)p;      break; /* north */
                case 2: x -= (seq_t)p;      break; /* west */
                case 3: y -= (seq_t)p;      break; /* south */
            }

            /* Store coordinates after this step. */
            cache[2 * (size_t)k + 2] = x;
            cache[2 * (size_t)k + 3] = y;
        }
        computed_steps = step_needed;
    }

    /* Return the required entry. */
    return cache[(size_t)n];
}
