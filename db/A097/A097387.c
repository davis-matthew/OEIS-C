/*
 * A097387  Let f(x)=(largest digit of x)^(smallest digit of x) + x (A097385).
 *          Sequence gives numbers n such that f(n) and f(n+1) are both prime.
 * https://oeis.org/A097387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_seq_array, big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097387: numbers n for which f(n) and f(n+1) are prime,
    where f(x) = (largest digit of x)^(smallest digit of x) + x.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

/* return true iff seq_t number x satisfies f(x) prime and f(x+1) prime. */
static bool is_candidate(seq_t x)
{
    seq_t t = x;
    unsigned largest = 0, smallest = 9;

    if (t == 0) {
        largest = smallest = 0;
    } else {
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            t /= 10;
        }
    }

    seq_t fx = x + big_pow((seq_t)largest, (seq_t)smallest);
    if (!is_prime_big(fx))
        return false;

    seq_t xn = x + 1;
    t = xn;
    largest = 0;
    smallest = 9;
    if (t == 0) {
        largest = smallest = 0;
    } else {
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            t /= 10;
        }
    }
    seq_t fn = xn + big_pow((seq_t)largest, (seq_t)smallest);
    return is_prime_big(fn);
}

/* Return the nth term of the sequence (a(1) is the first listed term). */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_start = 1;        /* next integer to test */

    /* initialise cache if needed */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = alloc_seq_array(cache_cap);
    }

    while (cache_len < n) {
        seq_t cand = (seq_t)next_start;

        if (is_candidate(cand)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
#if defined(__cplusplus)
                seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_start;
    }

    return cache[n - 1];
}
