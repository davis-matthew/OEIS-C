/*
 * A168500  Primes n of the form 10000p+100q+r with primes p, q and r,
 *          98>p>q>r>10.
 * https://oeis.org/A168500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth prime of the form 10000p+100q+r where p,q,r are
 *            two‑digit primes with 98>p>q>r>10
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: full, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static int cmp_seq_generic(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a > b) - (a < b);
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool initialized = false;

static void build_cache(void) {
    /* gather all two‑digit primes >10 and <100 */
    uint64_t two[25];
    size_t two_len = 0;
    for (seq_t x = 11; x < 100; ++x) {
        if (is_prime_seq((seq_t)x))
            two[two_len++] = (uint64_t)x;
    }

    /* dynamic array to hold valid concatenations */
    size_t cap = 0, len = 0;
    seq_t *arr = NULL;

    for (size_t i = 0; i < two_len; ++i) {
        for (size_t j = 0; j < i; ++j) {
            for (size_t k = 0; k < j; ++k) {
                seq_t val = (seq_t)10000 * (seq_t)two[i]
                          + (seq_t)100   * (seq_t)two[j]
                          +                (seq_t)two[k];
                if (is_prime_seq(val)) {
                    if (len == cap) {
                        cap = cap ? cap * 2 : 8;
                        arr = mem_resize(seq_t, arr, cap);
                    }
                    arr[len++] = val;
                }
            }
        }
    }

    /* sort ascending */
    mem_qsort(arr, len, sizeof(seq_t), cmp_seq_generic);

    /* store in static cache */
    mem_free(cache);
    cache = arr;
    cache_len = len;
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized)
        build_cache();

    if (n == 0 || n > cache_len)
        return 0;            /* undefined for out of bounds */

    return cache[(size_t)(n - 1)];
}
