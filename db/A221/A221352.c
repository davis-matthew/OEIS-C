/*
 * A221352  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, without
 *          2-loops.
 * https://oeis.org/A221352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) -26*a(n-2) -10*a(n-3) +34*a(n-4) +14*a(n-5)
 *            -16*a(n-6) -8*a(n-7) Initial values:
 *            a(1)=0,a(2)=1,a(3)=18,a(4)=148,a(5)=1164,a(6)=8824,a(7)=65932
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cached values a[0..cap-1]   */
static size_t cap = 0;               /* allocated size             */
static idx_t computed_max = 0;       /* highest index computed in cache */
static bool init_done = false;       /* base values initialised    */

/* initial values a(1) .. a(7); index 0 unused */
static const seq_t init_vals[8] = {0, 0, 1, 18, 148, 1164, 8824, 65932};

/* Ensure the cache can hold indices up to 'need' (inclusive) and
   initialise the first 7 values if not already done. */
static void
ensure_cache(idx_t need)
{
    size_t new_cap = (size_t)(need + 1);
    if (new_cap < 8) new_cap = 8;                       /* enough for seeds 1..7 */

    if (cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }                         /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    if (!init_done) {
        cache[0] = (seq_t)0;                            /* optional */
        for (idx_t i = 1; i <= 7; ++i) {
            cache[i] = init_vals[i];
        }
        computed_max = 7;
        init_done = true;
    }
}

/* public function */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;                       /* undefined, but harmless */

    ensure_cache(n);

    /* Return if already computed */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Compute values for indices > computed_max up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* positive contributions */
        pos += (seq_t)11 * cache[i - 1];
        pos += (seq_t)34 * cache[i - 4];
        pos += (seq_t)14 * cache[i - 5];

        /* negative contributions */
        neg += (seq_t)26 * cache[i - 2];
        neg += (seq_t)10 * cache[i - 3];
        neg += (seq_t)16 * cache[i - 6];
        neg += (seq_t)8  * cache[i - 7];

        cache[i] = pos - neg;  /* result is guaranteed non‑negative */
    }
    computed_max = n;
    return cache[n];
}
