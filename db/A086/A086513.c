/*
 * A086513  Primes arising in A086512: a(n)= (2n-1)-th partial sum of A086512.
 * https://oeis.org/A086513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..2n-1} A086512(k); search: odd-indexed terms of
 *            A086512 are the smallest unused number making the partial sum
 *            prime, even-indexed terms the smallest unused number
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static void A086513_ensure(size_t needed, unsigned char **used, size_t *cap)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap == 0) ? 64 : *cap;
    while (needed >= new_cap) new_cap <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(*used, new_cap);
    if (!tmp) abort();
    memset(tmp + *cap, 0, new_cap - *cap);
    *used = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static unsigned char *used = NULL;
    static size_t used_cap = 0;
    static idx_t sum = 0;
    static size_t low = 1; /* smallest unused number */

    if (n == 0) return (seq_t)0;

    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    while (cache_len < (size_t)n) {
        /* odd-indexed term: smallest unused cand with prime partial sum */
        size_t cand = low;
        for (;;) {
            A086513_ensure(cand, &used, &used_cap);
            if (!used[cand] && is_prime(sum + (idx_t)cand)) break;
            ++cand;
        }
        used[cand] = 1;
        sum += (idx_t)cand;
        cache[cache_len] = (seq_t)sum;
        ++cache_len;
        /* even-indexed term: smallest unused */
        for (;;) {
            A086513_ensure(low, &used, &used_cap);
            if (!used[low]) break;
            ++low;
        }
        used[low] = 1;
        sum += (idx_t)low;
    }
    return cache[(size_t)n - 1];
}
