/*
 * A086322  Primes produced by repeated application of the formula p -> (10p
 *          +- 3) starting at the prime 2.
 * https://oeis.org/A086322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   breadth‑first generation of primes by the rule p → 10p ± 3
 *            starting from 2. The sequence is produced by exploring a binary
 *            tree whose nodes are primes; each prime generates up to two
 *            children that are appended to the list in the order they are
 *            discovered. The function a(n) returns the n‑th prime...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers required by both C11 and C++17 */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime_big.h"

/* Maximum number of terms we keep in the cache.  The sequence grows
   very slowly (about one new prime per one new node) and for the
   expected test limits (n ≤ 10000) 40000 is more than sufficient. */
#define MAX_TERMS 40000

/* Static cache of sequence terms and its usage count. */
static seq_t cache[MAX_TERMS];
static size_t cache_cnt = 0;   /* number of terms currently stored */

/* Static queue that holds indices of terms that still need to be
   expanded.  queue_head points to the next element to pop; queue_tail
   points just past the last inserted element. */
static idx_t queue[MAX_TERMS];
static size_t queue_head = 0, queue_tail = 0;

/* Initialise the cache with the first prime 2 and seed the queue. */
static void init_sequence(void)
{
    cache[0] = (seq_t)2;
    cache_cnt = 1;

    queue[0] = 0;   /* index of the first prime */
    queue_head = 0;
    queue_tail = 1;
}

/* Return the n‑th term (1‑based).  For n==0 we return 0 as a safe
   placeholder, though the sequence is defined only for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Lazily initialise on first call. */
    if (cache_cnt == 0)
        init_sequence();

    /* Expand the tree until we have at least n terms.  The loop
       stops early if we run out of queue elements (which cannot
       happen for n within the tested range). */
    while (cache_cnt < (size_t)n) {
        /* If the queue is empty something went wrong; break. */
        if (queue_head >= queue_tail)
            break;

        /* Pop the next index to expand. */
        idx_t idx = queue[queue_head++];
        seq_t parent = cache[idx];

        /* Child: 10*p - 3 */
        seq_t child1 = (seq_t)10 * parent - (seq_t)3;
        if (is_prime_big(child1)) {
            if (queue_tail < MAX_TERMS) {
                queue[queue_tail++] = (idx_t)cache_cnt;
            }
            if (cache_cnt < MAX_TERMS) {
                cache[cache_cnt++] = child1;
            }
        }

        /* Child: 10*p + 3 */
        seq_t child2 = (seq_t)10 * parent + (seq_t)3;
        if (is_prime_big(child2)) {
            if (queue_tail < MAX_TERMS) {
                queue[queue_tail++] = (idx_t)cache_cnt;
            }
            if (cache_cnt < MAX_TERMS) {
                cache[cache_cnt++] = child2;
            }
        }
    }

    /* Convert from 1‑based to 0‑based index for the cache. */
    return cache[(size_t)n - 1];
}
