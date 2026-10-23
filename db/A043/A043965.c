/*
 * A043965  Numbers k such that 0 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A043965
   a(n) = the n‑th integer k whose base‑9 representation contains
   adjacent digits 0 and 6 (either 06 or 60) but whose base‑9
   representation of k+1 does not.  The sequence starts with
   a(1)=54, a(2)=87, a(3)=135, … */

#include <stdbool.h>
#include <stdlib.h>

static bool has_adjacent_06_60(seq_t x)
{
    seq_t prev = (seq_t)-1;          /* sentinel: no previous digit */
    while (x > 0) {
        seq_t d = x % 9;             /* least significant digit in base‑9 */
        if (((d == 0) && (prev == 6)) || ((d == 6) && (prev == 0))) {
            return true;
        }
        prev = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /*   cache[n-1] holds the n-th term (offset 1) */
    static seq_t *cache = NULL;
    static idx_t cache_size  = 0;    /* number of terms stored */
    static idx_t cache_cap   = 0;    /* allocated capacity */
    static seq_t last_checked = 0;   /* largest integer tested so far */

    if (n == 0) return 0;           /* offset starts at 1 */

    while (cache_size < n) {
        seq_t k = last_checked;
        ++k;                    /* k = last_checked + 1 */
        last_checked = k;

        seq_t kp1 = k;
        ++kp1;                  /* kp1 = k + 1 */

        if (has_adjacent_06_60(k) && !has_adjacent_06_60(kp1)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                        cache_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
            }
            cache[cache_size++] = k;
            if (cache_size == n)   /* reached the required index */
                return k;
        }
    }

    return cache[n - 1];
}
