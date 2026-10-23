/*
 * A172508  Prime numbers such that the differences between any pair of digits
 *          is prime.
 * https://oeis.org/A172508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A172508: primes such that the difference between any pair of
 * decimal digits is itself a prime (2, 3, 5, or 7).  The function searches
 * primes sequentially, keeps a static cache, and returns the n‑th term
 * (a(1) = 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/get_digits.h"

/* Return true if all decimal digits of x are distinct and every
 * pairwise difference is one of {2,3,5,7}. */
static bool good_digits(const seq_t x)
{
    uint8_t buf[300];
    size_t len;

    get_digits(x, buf, &len);

    if (len > 5)                /* at most 5 distinct digits possible */
        return false;

    bool seen[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (seen[d])
            return false;
        seen[d] = true;
    }

    const int primeDiff[4] = {2, 3, 5, 7};

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            int diff = (int)buf[i] > (int)buf[j]
                       ? (int)buf[i] - (int)buf[j]
                       : (int)buf[j] - (int)buf[i];
            bool ok = false;
            for (int k = 0; k < 4; ++k)
                if (diff == primeDiff[k]) { ok = true; break; }
            if (!ok)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    /* static fields used during generation */
    static bool initialized = false;
    static seq_t candidate;

    if (n == (idx_t)0)   /* undefined for offset < 1 */
        return (seq_t)0;

    if (!initialized) {
        candidate   = (seq_t)2;  /* first prime to test */
        initialized = true;
    }

    while (size < n) {
        /* test candidate if it is 2 or odd */
        if (candidate == (seq_t)2 || (candidate % (seq_t)2 != (seq_t)0)) {
            if (is_prime_big(candidate) && good_digits(candidate)) {
                if (size + 1 > cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)  /* allocation failure – abort */
                        return (seq_t)0;
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[size++] = candidate;
            }
        }

        /* advance to the next integer */
        if (candidate == (seq_t)2)
            candidate = (seq_t)3;
        else
            candidate = candidate + (seq_t)2;   /* only odd numbers thereafter */
    }

    return cache[n - 1];
}
