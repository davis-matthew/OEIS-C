/*
 * A130318  Integer values of k!!/S(k), where S(k) is the sum of all odd
 *          numbers less than or equal to k, if k is odd, or the sum of all
 *          even numbers less than or equal to k, if k is even.
 * https://oeis.org/A130318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = integer part of k!! / S(k) for the smallest k that yields
 *            a new term, with the first two terms equal to 1 and all later
 *            terms where the value equals 1 omitted. S(k) is the sum of all
 *            odd numbers ≤ k if k is odd, or the sum of all even numbers ≤ k
 *            if k is even. (k!! is the double factorial)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t   *cache = NULL;      /* cached terms  */
static size_t  cache_cap = 0;       /* capacity of cache   */
static size_t  cache_len = 0;       /* number of stored terms */
static idx_t   next_k   = 0;        /* next k to process   */
static size_t  ones_seen = 0;       /* number of '1' terms stored */

/* enlarge cache if necessary */
static void ensure_cache_space(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();          /* out of memory */
        cache = newp;
        cache_cap = new_cap;
    }
}

static seq_t term_of_k(idx_t k)
{
    if (k == 0) return 1;           /* special base case */

    seq_t denom;
    if (k & 1U) {                  /* odd k */
        seq_t m = ((seq_t)k + 1) / 2;
        denom = m * m;
    } else {                       /* even k */
        seq_t t = (seq_t)k / 2;
        denom = t * (t + 1);
    }

    seq_t num   = 1;
    idx_t j     = k;

    while (j > 0) {
        seq_t factor = (seq_t)j;
        seq_t g = gcd(denom, factor);
        denom   /= g;
        factor  /= g;
        num      *= factor;

        if (j > 2) j -= 2;
        else break;                /* done with last factor */
    }

    /* if the division is exact, denom should be 1 */
    if (denom != 1) return 0;       /* not an integer value */
    return num;
}

seq_t a(idx_t n)
{
    /* answer already computed? */
    if (cache_len > (size_t)n)
        return cache[n];

    /* compute new terms until we reach index n */
    while (cache_len <= (size_t)n) {
        seq_t val = term_of_k(next_k);

        /* skip non‑integer values */
        if (val != 0) {
            /* skip later occurrences of the value 1 after the first two */
            if (val == 1) {
                if (ones_seen < 2) {
                    ensure_cache_space();
                    cache[cache_len++] = val;
                    ones_seen++;
                }
            } else {
                ensure_cache_space();
                cache[cache_len++] = val;
            }
        }
        next_k++;   /* advance to next k */
    }
    return cache[n];
}
