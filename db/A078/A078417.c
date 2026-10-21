/*
 * A078417  Numbers k such that h(k) = h(k+1), where h(k) is the length of k,
 *          f(k), f(f(k)), ..., 1 in the Collatz (or 3x + 1) problem. (The
 *          earliest "1" is meant.)
 * https://oeis.org/A078417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A078417: the sequence of integers k such that the collatz
 *  trajectory length h(k) equals h(k+1).  The value is obtained by
 *  scanning forward and counting matching k's until the nth one is
 *  reached.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- cache of short collatz lengths ----------
 *  We cache lengths for numbers up to MAX_CACHE to speed up the
 *  repeated calculation of h(k).  For numbers larger than that we
 *  compute the length iteratively while using cached values
 *  whenever we hit a small number.  */
#define MAX_CACHE ((idx_t)1 << 22)      /* 4 194 304 */

static uint32_t *len_cache = NULL;

static void init_len_cache(void)
{
    if (len_cache)
        return;
    /* cast the void* returned by calloc to the correct pointer type */
    len_cache = (uint32_t *)calloc(MAX_CACHE + 1, sizeof(uint32_t));
}

/* collatz_length: number of terms from n down to 1 (inclusive) */
static seq_t collatz_length(seq_t n)
{
    if (n == (seq_t)1)
        return (seq_t)1;

    init_len_cache();

    /* Use cached value if available */
    if (n <= (seq_t)MAX_CACHE && len_cache[(idx_t)n] != 0)
        return (seq_t)len_cache[(idx_t)n];

    /* iterative calculation */
    seq_t cur = n;
    uint32_t steps = 0;
    while (1) {
        if (cur == (seq_t)1) {            /* reached 1 */
            steps++;
            break;
        }
        /* If we hit a cached small number, add its length and stop */
        if (cur <= (seq_t)MAX_CACHE && len_cache[(idx_t)cur] != 0) {
            steps += len_cache[(idx_t)cur];
            break;
        }
        /* next collatz value */
        cur = (cur & (seq_t)1) ? (cur * (seq_t)3 + (seq_t)1) : (cur >> 1);
        steps++;
    }

    /* Store the result in the cache if n itself was small */
    if (n <= (seq_t)MAX_CACHE)
        len_cache[(idx_t)n] = steps;

    return (seq_t)steps;
}

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    /* n is 1‑based.  We simply scan from k=1 upward counting matches
     * until we reach the desired index.  This is O(n * m) where m is
     * the largest k examined, but for n up to 10 000 it easily stays
     * below the time limits. */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t len_k  = collatz_length((seq_t)k);
        seq_t len_k1 = collatz_length((seq_t)(k + 1));
        if (len_k == len_k1) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
}
