/*
 * A210080  Number of (n+1)X3 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having one, three or four distinct values, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A210080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 322*a(n-2) + 274*a(n-3) + 1887*a(n-4) -
 *            3124*a(n-5) + 76*a(n-6) + 1168*a(n-7) Initial values: a(1)=51,
 *            a(2)=2384, a(3)=82691, a(4)=2718363, a(5)=88297042,
 *            a(6)=2860160370, a(7)=92588511903
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0) return 0;

    /* Known initial values. */
    static const seq_t init[7] = {
        51ULL,
        2384ULL,
        82691ULL,
        2718363ULL,
        88297042ULL,
        2860160370ULL,
        92588511903ULL
    };

    if (n <= 7) return init[n-1];

    /* Static cache to store already computed terms. */
    static seq_t *cache = NULL;          /* array of computed values */
    static size_t  cap   = 0;            /* allocated capacity */
    static idx_t   size  = 0;            /* number of valid terms in cache */

    /* Initialise cache with the first seven terms if required. */
    if (!cache || size < 7) {
        size_t init_cap = 8;  /* at least 8 to hold a(1)..a(7) plus space */
        cache = (seq_t*)realloc(cache, init_cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        cap  = init_cap;
        size = 7;
    }

    /* Grow cache if the requested term is beyond current capacity. */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to n, using the recurrence. */
    for (idx_t i = size + 1; i <= n; ++i) {
        /* i refers to the index of a(i) (1‑based). */
        idx_t j = i - 1;          /* 0‑based index in cache */
        seq_t val = ((seq_t)42)  * cache[j - 1];
        val -= ((seq_t)322) * cache[j - 2];
        val += ((seq_t)274) * cache[j - 3];
        val += ((seq_t)1887) * cache[j - 4];
        val -= ((seq_t)3124) * cache[j - 5];
        val += ((seq_t)76)   * cache[j - 6];
        val += ((seq_t)1168) * cache[j - 7];
        cache[j] = val;
    }

    size = n;     /* cache now contains terms up to a(n) */
    return cache[n - 1];
}
