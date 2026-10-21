/*
 * A219256  Numbers whose base 64 representation does not contain any of the
 *          digits 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 16, 17, 18, 24, 25, 32,
 *          34, 36, 40, 44, 48, 50.
 * https://oeis.org/A219256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219256: numbers whose base‑64 representation uses only the digits
   10,11,13,14,15,19,20,21,22,23,26,27,28,29,30,31,33,35,37,38,39,41,42,43,45,46,47,49,51,52,53,54,55,56,57,58,59,60,61,62,63.
   Implementation scans integers and checks each base‑64 digit. */

#include <stdbool.h>
#include <stdlib.h>

/* allowed[d] is true iff digit d (0 ≤ d < 64) is allowed. */
static const bool allowed[64] = {
    0,0,0,0,0,0,0,0,0,0,          /* 0 – 9  */
    1,1,0,1,1,1,0,0,0,1,          /*10 –19  */
    1,1,1,1,0,0,1,1,1,1,1,1,      /*20 –31  */
    0,1,0,1,0,1,1,1,0,1,1,1,      /*32 –43  */
    0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1   /*44 –63  */
};

static inline bool is_allowed(seq_t val)
{
    seq_t x = val;
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 64);
        if (!allowed[d]) return false;
        x /= 64;
    }
    return true;
}

/* Cache of previously found terms to support out‑of‑order calls. */
static seq_t *cache   = NULL;     /* array of found values */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static size_t cache_len = 0;      /* number of terms stored in cache */
static seq_t next_val   = 10;     /* next integer to test for membership */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value for n<1 is undefined */
    while (cache_len < n) {
        /* Skip numbers that contain a forbidden digit. */
        while (!is_allowed(next_val))
            next_val++;

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store next allowed number. */
        cache[cache_len++] = next_val;
        next_val++;                            /* advance to the next integer */
    }
    return cache[n - 1];
}
