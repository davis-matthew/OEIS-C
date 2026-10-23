/*
 * A041781  Denominators of continued fraction convergents to sqrt(411).
 * https://oeis.org/A041781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 99460*a(n-10) - a(n-20) for n > 19 Initial values:
 *            a(0)=1, a(1)=3, a(2)=4, a(3)=7, a(4)=11, a(5)=216, a(6)=227,
 *            a(7)=443, a(8)=670, a(9)=2453, a(10)=98790, a(11)=298823,
 *            a(12)=397613, a(13)=696436, a(14)=1094049, a(15)=21483367,
 *            a(16)=22577416, a(17)=44060783, a(18)=66638199, a(19)=243975380
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   _alloc_cache_seq, _free_cache_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_alloc_cache_seq.h"
#include "../utils/_free_cache_seq.h"

/* Array of first 20 terms as unsigned long long for easy conversion to seq_t */
static const unsigned long long INIT[20] = {
    1ULL, 3ULL, 4ULL, 7ULL, 11ULL,
    216ULL, 227ULL, 443ULL, 670ULL, 2453ULL,
    98790ULL, 298823ULL, 397613ULL, 696436ULL, 1094049ULL,
    21483367ULL, 22577416ULL, 44060783ULL, 66638199ULL, 243975380ULL
};

#if defined(__cplusplus)

#else

#endif

/* Static cache for memoisation */
static seq_t * _cache_seq_ptr = NULL;
static size_t _cache_seq_cap = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > _cache_seq_cap) {
        seq_t *new_cache = _alloc_cache_seq(needed);
        if (!new_cache) return (seq_t)0; /* out of memory, fallback */
        if (_cache_seq_ptr) _free_cache_seq(_cache_seq_ptr);
        _cache_seq_ptr = new_cache;
        _cache_seq_cap = needed;
    }

    /* Compute (and overwrite) all terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 20) {
            _cache_seq_ptr[(size_t)i] = (seq_t)INIT[(size_t)i];
        } else {
            _cache_seq_ptr[(size_t)i] =
                (seq_t)99460 * _cache_seq_ptr[(size_t)(i - 10)]
                - _cache_seq_ptr[(size_t)(i - 20)];
        }
    }
    return _cache_seq_ptr[(size_t)n];
}
