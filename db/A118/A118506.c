/*
 * A118506  Smaller of two consecutive Sophie Germain primes with the same
 *          digital sum.
 * https://oeis.org/A118506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118506: Smaller of two consecutive Sophie Germain primes with the same digital sum.
   The implementation searches for Sophie Germain primes in ascending order,
   compares the digital sums of each pair of consecutive SG primes,
   and stores the smaller one of each matching pair in a cache.
   Subsequent calls are served from the cache, allowing random access
   with an average O(1) lookup after the needed terms have been generated. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;      /* cache of computed terms               */
static size_t seq_len   = 0;           /* number of elements stored             */
static size_t seq_cap   = 0;           /* current capacity of the cache         */
static uint64_t cur_prime = 3;          /* odd integer to test next               */
static uint64_t prev_sg   = 0;          /* previous SG prime found                 */
static bool     has_prev  = false;      /* true if a previous SG prime has been seen */

/* Expand the cache if needed. */
static void ensure_capacity(size_t need) {
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = seq_realloc(seq_cache, seq_cap, new_cap);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Append a new term to the cache. */
static void append_term(seq_t val) {
    ensure_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Find the next Sophie Germain prime, update global state,
   and record a term if the current and previous SG primes
   share the same digit sum. */
static void find_next_sg_prime(void) {
    while (1) {
        uint64_t p = cur_prime;           /* candidate SG prime              */
        cur_prime += 2;                   /* next odd number                */
        if (!is_prime(p)) continue;           /* p must be prime                 */
        uint64_t q = (p << 1) + 1;        /* q = 2p + 1                      */
        if (!is_prime(q)) continue;           /* q must also be prime            */

        /* p is a Sophie Germain prime */
        if (has_prev) {
            seq_t sum_prev = digit_sum((seq_t)prev_sg);
            seq_t sum_cur  = digit_sum((seq_t)p);
            if (sum_prev == sum_cur) {
                append_term((seq_t)prev_sg);  /* smaller member of the pair   */
            }
        }
        prev_sg = p;
        has_prev = true;
        return;                /* one SG prime processed per call */
    }
}

/* External interface expected by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for n < 1: return 0 */
    while (seq_len < (size_t)n) find_next_sg_prime();
    return seq_cache[(size_t)n - 1];
}
