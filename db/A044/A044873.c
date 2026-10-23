/*
 * A044873  Numbers having, in base 2, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A044873:
   numbers whose binary run‑length sum of even‑length runs equals sum
   of odd‑length runs.  The function builds the sequence on demand
   and caches all previously found terms, so that subsequent calls are
   fast regardless of the order of n.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool is_balanced(seq_t x)
{
    if (x == 0)
        return false;   /* 0 is not in the sequence */
    seq_t tmp = x;
    int prev_bit = -1;
    int run = 0;
    seq_t sum_even = 0, sum_odd = 0;

    while (tmp) {
        int bit = (int)(tmp & 1);
        if (prev_bit != bit) {
            if (prev_bit != -1) {                /* finish previous run */
                if (run & 1)
                    sum_odd += (seq_t)run;
                else
                    sum_even += (seq_t)run;
            }
            prev_bit = bit;
            run = 1;
        } else {
            run++;
        }
        tmp >>= 1;
    }
    /* finish last run */
    if (run) {
        if (run & 1)
            sum_odd += (seq_t)run;
        else
            sum_even += (seq_t)run;
    }
    return sum_even == sum_odd;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t last_checked = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE); /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for zero index, but safe return */

    /* generate terms until we have at least n terms cached */
    while ((size_t)cache_len < (size_t)n) {
        last_checked++;
        if (is_balanced(last_checked)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len] = last_checked;
            cache_len++;
        }
    }
    return cache[n - 1];
}
