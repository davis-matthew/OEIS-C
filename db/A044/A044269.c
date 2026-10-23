/*
 * A044269  Numbers n such that string 2,0 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑9 representation of n contains
   the substring "20" but the base‑9 representation of n-1 does not.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has20.h"

/* Cache of found terms */
static seq_t *_cache = NULL;
static size_t _cap = 0;
static size_t _len = 0;
static seq_t _next = 1;   /* next candidate to test */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (_len < n) {
        seq_t cand = _next;
        if (has20(cand) && !has20(cand - 1)) {
            if (_len >= _cap) {
                size_t newcap = (_cap == 0) ? 64 : _cap * 2;
                seq_t *tmp = (seq_t *)realloc(_cache, newcap * sizeof(seq_t));
                _cache = tmp;
                _cap = newcap;
            }
            _cache[_len++] = cand;
        }
        _next++;
    }
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1, return 0 as a safe value */
    ensure_cache(n);
    return _cache[n - 1];
}
