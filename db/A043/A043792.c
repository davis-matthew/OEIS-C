/*
 * A043792  Numbers whose number of runs in base 3 is congruent to 1 (mod 7).
 * https://oeis.org/A043792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose number of runs in base 3 is congruent to 1 (mod 7).
   a(1) = 1, a(2) = 2, a(3) = 4, ... , a(24) = 2489
   The function generates terms by brute‑force search and caches the
   first 20000 values in a static array.  It is suitable for any
   build where seq_t is a trivial integer type or a big integer
   class (since no dynamic allocation of seq_t objects is performed). */

#include <stdbool.h>

/* Count the number of runs in the base‑3 representation of x. */
static inline int run_count(seq_t x)
{
    int runs = 0;
    bool first = true;
    seq_t prev = 0;          /* will hold the previous digit */

    while (x > 0) {
        seq_t d = x % 3;
        if (first) {        /* first non‑zero digit */
            runs = 1;
            first = false;
        } else if (d != prev) {
            runs++;
        }
        prev = d;
        x /= 3;
    }
    return runs;
}

/* Cache for the first 20000 terms of the sequence. */
static seq_t  cache[20000];
static idx_t  cache_size = 0;  /* number of terms stored, 0…20000 */
static seq_t  cand       = 1;   /* next integer to test */

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1, but harmless */

    /* Ensure we have at least n terms in the cache. */
    while (cache_size < n) {
        if ((run_count(cand) % 7) == 1) {
            cache[cache_size++] = cand;
        }
        cand++;
        /* Safety: if the requested n exceeds the cache size, abort. */
        if (cache_size >= 20000 && n > 20000) {
            abort();
        }
    }
    return cache[n - 1];
}
