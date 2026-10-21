/*
 * A078177  Composite numbers with an integer arithmetic mean of all prime
 *          factors.
 * https://oeis.org/A078177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number whose prime factors (with
 *            multiplicity) have an integer arithmetic mean. The sequence is
 *            computed lazily by scanning successive integers and caching
 *            qualifying values. The algorithm is O(m) where m is the largest
 *            integer examined to produce the requested n‑th term
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined in the surrounding build environment. */

/* Cache of terms (1‑indexed). */
static seq_t *cache = NULL;
static size_t cap   = 0;          /* capacity of the cache */
static size_t len   = 0;          /* number of valid terms in cache */
static uint64_t last_checked = 3; /* last integer examined (inclusive) */

/* Append a new term, expanding the cache if necessary. */
static void append_term(seq_t val)
{
    if (len >= cap) {
        size_t new_cap = (cap == 0) ? 128 : cap * 2;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }
    cache[len++] = val;
}

/* Extend the cache until it contains at least 'need' terms. */
static void ensure_cache(idx_t need)
{
    while (len < need) {
        last_checked++;
        uint64_t n = last_checked;
        if (n < 4) continue;          /* skip 1,2,3 – not composite */

        uint64_t sum = 0;   /* sum of prime factors (with multiplicity) */
        uint64_t mult = 0;  /* total count of prime factors (with multiplicity) */
        uint64_t temp = n;

        /* factor 2 separately */
        if ((temp & 1u) == 0u) {
            uint64_t cnt = 0;
            while ((temp & 1u) == 0u) {
                temp >>= 1;
                cnt++;
            }
            sum += 2 * cnt;
            mult += cnt;
        }

        /* odd factors up to sqrt(temp) */
        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                uint64_t cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                sum += p * cnt;
                mult += cnt;
            }
        }

        /* remaining factor > 1 */
        if (temp > 1) {
            sum += temp;
            mult += 1;
        }

        /* add to cache if composite and mean is integer */
        if (mult > 1 && sum % mult == 0u) {
            append_term((seq_t)n);
        }
    }
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for index 0; harmless fallback */
    ensure_cache(n);
    return cache[n - 1];
}
