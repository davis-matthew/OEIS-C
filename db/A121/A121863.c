/*
 * A121863  See Comments lines for definition.
 * https://oeis.org/A121863
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(4)=16, for n≥5: a(n)= interpret decimal digits of 2^n in base
 *            a(n-1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   alloc_seq, free_seq_array, seq_mul, seq_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;   /* values below the offset are irrelevant */

    /* cache of already computed terms */
    static seq_t *cache   = NULL;    /* cache[0..computed_until] */
    static idx_t  size    = 0;
    static idx_t  computed_until = 3;   /* last computed index */

    /* static value of 2^i for reuse between calls */
    static seq_t pow2  = (seq_t)0;
    static idx_t pow2_exp = 0;

    /* initialise static data on first call */
    if (!cache) {
        size = n + 5;                     /* at least n+1 for the requested index */
        cache = alloc_seq(size);
        memset(cache, 0, size * sizeof(seq_t));

        /* a(4) is the first real term */
        cache[4] = (seq_t)16;
        computed_until = 4;

        /* initialise pow2 = 2^4 */
        pow2  = seq_pow((seq_t)2, 4);
        pow2_exp = 4;
    }

    /* grow cache if necessary */
    if (size <= n) {
        idx_t new_size = n + 5;
        seq_t *new_cache = alloc_seq(new_size);
        memcpy(new_cache, cache, size * sizeof(seq_t));
        memset(new_cache + size, 0, (new_size - size) * sizeof(seq_t));
        free_seq_array(cache);
        cache = new_cache;
        size = new_size;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        if (i == 4) {                  /* already initialised */
            continue;
        }

        /* make sure pow2 == 2^i */
        while (pow2_exp < i) {
            pow2 = seq_mul(pow2, (seq_t)2);
            ++pow2_exp;
        }

        /* base for the interpretation is a(i-1) */
        seq_t base = cache[i - 1];

        /* estimate buffer size: 2^i has at most i+1 digits */
        size_t buf_len = (size_t)i + 5;
        char *buf = (char *)malloc(buf_len);
        if (!buf) { free_seq_array(cache); return 0; }

        size_t written;
        seq_to_str(pow2, buf, &written);   /* buf is null‑terminated */

        /* parse the decimal string in the given base */
        seq_t val = (seq_t)0;
        for (size_t j = 0; j < written; ++j) {
            unsigned d = (unsigned)(buf[j] - '0');
            val = seq_mul(val, base);
            val += (seq_t)d;
        }

        cache[i] = val;
        free(buf);
    }

    computed_until = (computed_until > n) ? computed_until : n;
    return cache[n];
}
