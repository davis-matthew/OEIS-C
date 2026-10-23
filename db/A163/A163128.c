/*
 * A163128  a(n) is the n-th self-number minus n.
 * https://oeis.org/A163128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003052: self numbers (numbers not of the form k + digit_sum(k)).
   a(n) = A003052(n) - n.  a(1) is the first term (1 - 1 = 0).  */
/* The algorithm generates the numbers k in increasing order, marks all
   values f(k) = k + digit_sum(k) in a boolean array `gen`.  Whenever a
   number k is seen that has not been marked, it is a self‑number and
   is stored in the array `self_nums`.  When we have stored at least n
   self‑numbers, the n‑th one is returned minus n.  The arrays are
   expanded only when necessary; new memory is zero‑initialized so that
   the boolean values start false.  This works for any of the supported
   integer types because the only operations are integer arithmetic
   and array indexing into a 64‑bit index space.  */
#include <stdlib.h>
#include <stdbool.h>

/* State needed for memoisation */
static seq_t *self_nums   = NULL;   /* stored self‑numbers */
static size_t self_cap    = 0;        /* capacity of self_nums */
static size_t self_cnt    = 0;        /* number of stored self‑numbers */

static bool *gen          = NULL;    /* generated marker array */
static size_t gen_cap     = 0;        /* capacity of gen */

/* current number to process */
static idx_t next_k = 1;

/* ensure self_nums has capacity at least `min_cap` */
static void ensure_self_capacity(size_t min_cap)
{
    if (min_cap <= self_cap) return;
    size_t new_cap = self_cap ? self_cap * 2 : 8;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(self_nums, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure; caller will abort */
    self_nums = tmp;
    self_cap = new_cap;
}

/* ensure gen array has capacity at least `min_cap` (zero‑initialised) */
static void ensure_gen_capacity(size_t min_cap)
{
    if (min_cap <= gen_cap) return;
    size_t new_cap = gen_cap ? gen_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    bool *tmp = (bool *)realloc(gen, new_cap * sizeof(bool));
    if (!tmp) return;
    /* initialise new part to false */
    if (new_cap > gen_cap)
        memset(tmp + gen_cap, 0, (new_cap - gen_cap) * sizeof(bool));
    gen = tmp;
    gen_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0, but never called */

    /* generate until we have at least n self‑numbers */
    while (self_cnt < (size_t)n) {
        /* make sure we can look at k itself */
        ensure_gen_capacity((size_t)next_k + 1);

        /* if k has not been generated yet, it is a self‑number */
        if (!gen[(size_t)next_k]) {
            ensure_self_capacity(self_cnt + 1);
            self_nums[self_cnt] = (seq_t)next_k;
            self_cnt++;
        }

        /* compute f(k) = k + digit_sum(k) and mark it as generated */
        idx_t tmp = next_k;
        seq_t sum = 0;
        while (tmp) {
            sum += tmp % 10;
            tmp /= 10;
        }
        seq_t f = (seq_t)next_k + sum;
        size_t f_idx = (size_t)f;
        ensure_gen_capacity(f_idx + 1);
        gen[f_idx] = true;

        next_k++;
    }

    /* n-th self‑number is stored at index n-1 */
    return self_nums[(size_t)n - 1] - (seq_t)n;
}
