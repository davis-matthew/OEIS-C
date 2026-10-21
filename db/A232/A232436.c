/*
 * A232436  Numbers which are uniquely decomposable into x^2+xy+y^2, the
 *          unique decomposition being with two distinct nonzero x and y.
 * https://oeis.org/A232436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest integer expressible as 3^a * p * q^2,
 *            where p is a prime ≡1 (mod 6) with exponent exactly 1, q is any
 *            integer whose prime factors are all ≡2 (mod 6), and the prime 3
 *            may appear with any exponent
 * Range:     u8 n <= 53, u16 n <= 8024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* helper: test whether m satisfies the characterization */
static bool is_valid(idx_t m)
{
    idx_t temp = m;
    int p1_count = 0;

    while (temp != 1) {
        /* handle factor 2 */
        if (temp % 2 == 0) {
            size_t e = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                e++;
            }
            if (e % 2 == 1)        /* 2 may only appear with even exponent */
                return false;
            continue;
        }

        /* find smallest odd divisor */
        idx_t p = 0;
        idx_t d = 3;
        while (d * d <= temp) {
            if (temp % d == 0) {
                p = d;
                break;
            }
            d += 2;
        }
        if (p == 0)              /* temp itself is prime */
            p = temp;

        /* get exponent of this prime */
        size_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }

        if (p == 3) {
            /* 3 is unrestricted */
        } else if (p % 6 == 1) {     /* prime of form 6k+1 must appear once */
            if (e != 1)
                return false;
            p1_count++;
            if (p1_count > 1)
                return false;
        } else { /* prime of form 6k+5 (or 2) must have even exponent */
            if (e % 2 == 1)
                return false;
        }
    }

    return p1_count == 1;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 0;

    if (n == 0)
        return 0; /* undefined, but keep safe */

    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_checked = 0;
    }

    /* generate terms until we have enough */
    while (cache_len < (size_t)n) {
        idx_t candidate = (last_checked == 0) ? 7 : ++last_checked;

        /* skip numbers <7 */
        if (candidate < 7)
            candidate = 7;

        /* advance until a valid term is found */
        while (!is_valid(candidate))
            ++candidate;

        /* store the term */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)candidate;
        last_checked = candidate;
    }

    return cache[n - 1];
}
