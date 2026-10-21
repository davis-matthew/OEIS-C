/*
 * A071764  Number of minimal rectangular envelopes (up to rotation) that
 *          enclose n contiguous squares.
 * https://oeis.org/A071764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + ceil(n/2) - ceil(tau(n-1)/2) (a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(k) is the number of divisors of k.  The implementation keeps
   an incremental cache: a_cache holds the sequence values, and
   tau_cache holds divisor counts computed by a sieve that is
   extended only as needed.  All arithmetic is integer; no
   floating points or math.h functions are used. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;          /* a_cache[i] = a(i) */
static uint64_t *tau_cache = NULL;    /* tau_cache[i] = tau(i) */
static idx_t   a_size   = 0;           /* size of a_cache (fields 0..a_size-1)  */
static idx_t   tau_size = 0;           /* size of tau_cache (fields 0..tau_size-1) */

/* Extend the divisor‑count cache to include indices up to (new_limit) inclusive.
   old_limit = tau_size-1 is the greatest index previously processed.      */
static void extend_tau_cache(idx_t new_limit)
{
    idx_t old_limit = (tau_size == 0) ? 0 : tau_size - 1;
    if (new_limit < old_limit) return;              /* nothing to do */

    /* grow the array if necessary */
    if (tau_size <= new_limit) {
        uint64_t *new_arr = (uint64_t *)realloc(tau_cache,
                                                (new_limit + 1) * sizeof(uint64_t));
        if (!new_arr) return;                       /* allocation failure – ignore */
        tau_cache = new_arr;
        if (tau_size == 0) {                        /* first allocation */
            /* initialise tau[0] = 1 so that a(1) can be computed */
            for (idx_t i = 0; i <= new_limit; ++i) tau_cache[i] = 0;
            tau_cache[0] = 1;
        }
        tau_size = new_limit + 1;
    }

    /* Incrementally update divisor counts for indices > old_limit */
    for (idx_t i = 1; i <= new_limit; ++i) {
        /* first multiple of i that is > old_limit */
        idx_t start = (old_limit + 1 + i - 1) / i * i;   /* ceil((old_limit+1)/i) * i */
        if (start < i) start = i;                       /* ensure we start at i for i>old_limit */
        for (idx_t j = start; j <= new_limit; j += i)
            ++tau_cache[j];
    }
}

/* Extend the sequence cache to include indices up to (new_limit) inclusive. */
static void extend_a_cache(idx_t new_limit)
{
    idx_t old_limit = (a_size == 0) ? 0 : a_size - 1;
    if (new_limit < old_limit) return;            /* nothing to do */

    /* grow the array if necessary */
    if (a_size <= new_limit) {
        seq_t *new_arr = (seq_t *)realloc(a_cache,
                                          (new_limit + 1) * sizeof(seq_t));
        if (!new_arr) return;                     /* allocation failure – ignore */
        a_cache = new_arr;
        a_size = new_limit + 1;
    }

    /* Sequentially compute missing terms using the recurrence */
    for (idx_t n = old_limit + 1; n <= new_limit; ++n) {
        seq_t prev   = a_cache[n - 1];
        seq_t ceil_half  = ((seq_t)n + 1) / 2;               /* ceil(n/2)   */
        seq_t ceil_tau   = ((seq_t)(tau_cache[n - 1] + 1)) / 2;/* ceil(tau(n-1)/2) */
        a_cache[n] = prev + ceil_half - ceil_tau;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* First ensure the caches are large enough */
    if (a_size == 0) {
        /* Initialise with a(0) = 1; tau(0) = 1 */
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        tau_cache = (uint64_t *)malloc(sizeof(uint64_t));
        if (!a_cache || !tau_cache) return (seq_t)0;   /* allocation failed */
        a_cache[0] = (seq_t)1;
        tau_cache[0] = 1;
        a_size = 1;
        tau_size = 1;
    }

    if (n >= a_size) {
        extend_tau_cache(n);         /* need tau up to n-1, and a up to n */
        extend_a_cache(n);
    }

    return a_cache[n];
}
