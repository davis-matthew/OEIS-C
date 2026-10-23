/*
 * A190799  Primes p=prime(i) such that prime(i+4)-prime(i)=20.
 * https://oeis.org/A190799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A190799 – primes p such that the difference between prime(i+4) and
    prime(i) equals 20.
    This implementation generates primes on demand, keeps the last five
    primes in a circular buffer and, whenever their span is exactly 20,
    records the first one as a term of the sequence.  The sequence values
    are cached in a dynamic array so that calls to a(n) are independent
    of call order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num); */

/* --- Static state ----------------------------------------------------- */
static uint64_t prime_buf[5];     /* last five primes (index 0 is earliest) */
static size_t buf_len = 0;        /* current count in prime_buf  (≤5) */

static seq_t *seq_vals = NULL;    /* cached sequence values */
static size_t seq_len = 0;        /* number of cached terms   */
static size_t seq_cap = 0;        /* allocated capacity of seq_vals */

static uint64_t cand = 3;         /* candidate odd number to test */
static bool initialized = false;

/* Grow the array for sequence values */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Add a new prime to the sliding window and, if it completes a
   qualifying 5‑prime block, store the first prime of that block. */
static void add_prime(uint64_t p)
{
    /* Shift buffer left if already full */
    if (buf_len == 5) {
        for (size_t i = 0; i < 4; ++i) prime_buf[i] = prime_buf[i + 1];
        prime_buf[4] = p;
    } else {
        prime_buf[buf_len++] = p;
        if (buf_len < 5) return;   /* not enough primes yet */
    }

    /* Now buf_len == 5, check gap */
    if (prime_buf[4] - prime_buf[0] == 20) {
        if (seq_len >= seq_cap) grow_seq();
        seq_vals[seq_len++] = (seq_t)prime_buf[0];
    }
}

/* One‑time setup: seed with the first prime */
static void init(void)
{
    if (initialized) return;
    init: /* label to satisfy compiler on some environments */
    add_prime(2);        /* prime[0] = 2 */
    cand = 3;            /* start testing from 3 */
    initialized = true;
}

/* Ensure that at least n terms are cached. */
static void ensure_seq(idx_t n)
{
    init();
    while (seq_len < (size_t)n) {
        /* Test current candidate */
        if (is_prime_small((seq_t)cand)) {
            add_prime(cand);
        }
        cand += 2;           /* only odd numbers after 2 */
    }
}

/* --- Public API ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range */
    ensure_seq(n);
    return seq_vals[n - 1];
}
