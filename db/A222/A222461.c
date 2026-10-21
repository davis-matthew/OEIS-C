/*
 * A222461  Number of nX5 0..7 arrays with no entry increasing mod 8 by 7
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence:
 *   a(n) = 10754*a(n-1) - 8060499*a(n-2)
 *        + 2219242223*a(n-3) - 245682627864*a(n-4)
 *        + 5798947687589*a(n-5)+448113231493438*a(n-6)
 *        - 2763020698450992*a(n-7)
 * Initial values:
 *   a(1)=2401, a(2)=23931607, a(3)=238535974201, a(4)=2377584520856755,
 *   a(5)=23698346512668445387, a(6)=236211004301185098853381,
 *   a(7)=2354410613553773007691517269
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static seq_t init_vals[8];
static bool init_done = false;

static void init_sequence(void)
{
    if (init_done) return;
    /* first four values fit in 64 bits */
    init_vals[1] = 2401;
    init_vals[2] = 23931607;
    init_vals[3] = 238535974201;
    init_vals[4] = 2377584520856755;
    if (sizeof(seq_t) > 8) {
        /* larger values only needed for big‑integer builds */
        init_vals[5] = seq_from_str("23698346512668445387");
        init_vals[6] = seq_from_str("236211004301185098853381");
        init_vals[7] = seq_from_str("2354410613553773007691517269");
    } else {
        init_vals[5] = 0;
        init_vals[6] = 0;
        init_vals[7] = 0;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t computed = 0;     /* highest index computed */

    if (n == 0) return 0; /* out of spec, safeguard */

    /* Ensure cache is allocated to hold up to n */
    if (!cache || (n + 1) > cache_size) {
        idx_t new_size = (n + 1 > 8) ? (n + 1) : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        if (new_size > cache_size) {
            /* initialize any new entries to zero */
            memset(cache + cache_size, 0,
                   (new_size - cache_size) * sizeof(seq_t));
            cache_size = new_size;
        }
    }

    /* Initialize constant values once */
    init_sequence();
    if (computed < 7) {
        for (idx_t i = 1; i <= 7 && i < cache_size; ++i)
            cache[i] = init_vals[i];
        computed = (cache_size > 7) ? 7 : cache_size - 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k < 8) continue; /* first 7 terms already set */
        seq_t t1 = cache[k - 1];
        seq_t t2 = cache[k - 2];
        seq_t t3 = cache[k - 3];
        seq_t t4 = cache[k - 4];
        seq_t t5 = cache[k - 5];
        seq_t t6 = cache[k - 6];
        seq_t t7 = cache[k - 7];
        cache[k] = 10754*t1 - 8060499*t2 + 2219242223*t3
                 - 245682627864*t4 + 5798947687589*t5
                 + 448113231493438*t6 - 2763020698450992*t7;
    }

    computed = n > computed ? n : computed;
    return cache[n];
}
