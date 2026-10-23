/*
 * A125238  Differences between consecutive deficient numbers.
 * https://oeis.org/A125238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A005100(n+1) - A005100(n) (difference of consecutive
 *            deficient numbers)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"

/* Array of deficient numbers already found.  Index 0 holds the first
   deficient number (1), index 1 the second (2), ... */
static seq_t *def_nums = NULL;
/* How many deficient numbers are currently stored. */
static size_t def_cnt = 0;
/* Current capacity of def_nums. */
static size_t def_cap = 0;
/* The next integer to test for deficiency.  After a deficient number x
   is found, last_checked is set to x+1. */
static idx_t last_checked = 1;

/* Grow the deficient‑number array when it runs out of space. */
static void grow_def_array(void)
{
    size_t new_cap = def_cap ? def_cap * 2 : 1024;
    seq_t *tmp = (seq_t *)realloc(def_nums, new_cap * sizeof(seq_t));
    if (!tmp && new_cap) abort();          /* out of memory – terminate */
    def_nums = tmp;
    def_cap  = new_cap;
}

/* Ensure that the array contains at least (n+1) deficient numbers
   (i.e. indexes 0 … n).  This will be called for every request of a(n). */
static void ensure_def_cnt(idx_t n)
{
    /* We need indices 0 … n, so keep generating deficient numbers
       until def_cnt > n. */
    while (def_cnt <= (size_t)n) {
        for (idx_t x = last_checked; ; ++x) {
            /* A number is deficient if its sum of divisors σ(x) <
               2·x.  The sigma_of function returns the sum of divisors. */
            if ((idx_t)sigma_of(x) < 2 * x) {
                if (def_cnt == def_cap) grow_def_array();
                def_nums[def_cnt++] = (seq_t)x;
                last_checked = x + 1;   /* next start point */
                break;                  /* go back to while test */
            }
        }
    }
}

/* Sequence A125238: difference between consecutive deficient numbers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n<1 – return 0 for safety */

    ensure_def_cnt(n);            /* guarantees def_nums[n] and def_nums[n-1] exist */
    return def_nums[(size_t)n] - def_nums[(size_t)n - 1];
}
