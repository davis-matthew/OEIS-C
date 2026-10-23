/*
 * A193571  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 42.
 * https://oeis.org/A193571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193571:
   a(n) = the n‑th smallest positive integer that, under repeated sum‑of‑squares‑of‑digits iteration,
   first reaches the cycle member 42 (before any other member of the cycle 4,16,37,58,89,145,20). */

#include <stdbool.h>
#include <stdlib.h>

/*--- auxiliary: sum of squares of decimal digits ---------------------------*/
static inline seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        seq_t d = x % 10;
        sum += d * d;
        x   /= 10;
    }
    return sum;
}

/*--- test whether the iteration first reaches 42 ------------------------*/
static inline bool first_entry_42(seq_t x)
{
    /* We'll walk until we hit 42 or any other cycle member or 1. */
    for (;;) {
        if (x == (seq_t)42)                  return true;
        if (x == (seq_t)4  ||
            x == (seq_t)16 ||
            x == (seq_t)37 ||
            x == (seq_t)58 ||
            x == (seq_t)89 ||
            x == (seq_t)145||
            x == (seq_t)20)                return false;
        if (x == (seq_t)1)                   return false;
        x = digit_square_sum(x);
    }
}

/*--- cache of already found terms ----------------------------------------*/
static seq_t  *cache          = NULL;   /* array storing the sequence values   */
static size_t  cache_len      = 0;      /* number of values currently stored  */
static size_t  cache_cap      = 0;      /* allocated capacity                  */
static idx_t   next_candidate = 1;      /* smallest integer not yet examined   */

/*--- grow the cache array to satisfy the requested size -----------------*/
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* memory allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/*--- public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n = 0; guard */

    /* generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* find the next integer that enters the cycle at 42. */
        while (1) {
            seq_t cand = (seq_t)next_candidate++;
            if (first_entry_42(cand)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
                break;
            }
        }
    }
    return cache[n - 1];
}
