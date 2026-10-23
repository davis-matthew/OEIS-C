/*
 * A168255  n appears n-th nonprime number times.
 * https://oeis.org/A168255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168255 – “n appears n‑th nonprime number times”
 *
 *  Each integer m occurs exactly T_m times where T_m is the
 *  m‑th nonprime number (sequence A018252).  The sequence
 *  a(n) is obtained by listing the integers 1, 2, 3, …, each
 *  repeated T_m times.  Thus a(n) is the smallest integer m
 *  such that
 *
 *      Σ_{k=1}^{m} T_k  ≥  n .
 *
 *  The implementation keeps a cumulative array of ΣT_k and
 *  expands it as needed.  A primality test from the utils
 *  library is used to generate the nonprime numbers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *cum = NULL;        /* cumulative sums of T_k = k‑th nonprime */
static size_t cum_count = 0;      /* number of cumulative sums stored */
static size_t cum_cap   = 0;      /* capacity of the cum array          */
static idx_t  candidate = 1;      /* next integer to test for nonprime  */

/* Ensure the cumulative array has space for at least one more element */
static void ensure_cum_capacity(void)
{
    if (cum_count >= cum_cap) {
        size_t new_cap = cum_cap ? cum_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* memory allocation failed */
        cum = tmp;
        cum_cap = new_cap;
    }
}

/* Return the next nonprime number (1 is treated as nonprime).
   After the call, `candidate` is set to the integer following this nonprime. */
static seq_t next_nonprime(void)
{
    for (;;) {
        if (candidate == 1 || !is_prime(candidate)) {
            seq_t val = (seq_t)candidate;
            candidate++;               /* advance past the found nonprime */
            return val;
        }
        candidate++;                   /* try the next integer */
    }
}

/* Generate the next cumulative sum and append it to the array */
static void add_next_sum(void)
{
    seq_t prev = cum_count ? cum[cum_count - 1] : (seq_t)0;
    seq_t nxt  = next_nonprime();
    seq_t sum  = prev + nxt;
    ensure_cum_capacity();
    cum[cum_count++] = sum;
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, but guard */
    /* Generate cumulative sums until they cover the requested index */
    while (cum_count == 0 || cum[cum_count - 1] < (seq_t)n) {
        add_next_sum();
    }
    /* Binary search to find minimal m with cum[m-1] ≥ n */
    size_t lo = 0, hi = cum_count - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cum[mid] < (seq_t)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)(lo + 1);   /* sequence values start at 1 */
}
