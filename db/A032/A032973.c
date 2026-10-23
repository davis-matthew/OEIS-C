/*
 * A032973  Numbers with the property that all pairs of consecutive digits
 *          differ by more than 1.
 * https://oeis.org/A032973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032973: numbers whose consecutive decimal digits differ by
   more than 1.  The function builds a cache of the first n terms on demand. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache ----------------------------------------------------------- */
static seq_t *cache = NULL;     /* array of found terms   */
static size_t cache_cap = 0;    /* array capacity        */
static idx_t cache_size = 0;    /* number of terms cached*/
static seq_t next_candidate = 1;/* next integer to test  */

/* enlarge cache if needed ------------------------------------------------ */
static void ensure_capacity(idx_t need) {
    if ((size_t)need <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 64;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* check if integer x satisfies the digit-difference condition --------- */
static bool good_number(seq_t x) {
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (prev != -1) {
            int diff = d - prev;
            if (diff < 0)
                diff = -diff;
            if (diff <= 1)
                return false;
        }
        prev = d;
        x /= 10;
    }
    return true;
}

/* allocate terms up to index n ------------------------------------------ */
static void grow_to(idx_t n) {
    while (cache_size < n) {
        if (good_number(next_candidate)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for n<1, but stay safe */
    grow_to(n);
    return cache[n - 1];
}
