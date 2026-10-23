/*
 * A031443  Digitally balanced numbers: positive numbers that in base 2 have
 *          the same number of 0's as 1's.
 * https://oeis.org/A031443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 8788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for A031443 (digitally balanced numbers). */
/* a(n) = the n‑th positive integer whose binary representation has
   the same number of 0's and 1's. The sequence is built lazily; an
   internal cache is expanded as needed. */

#include <stdlib.h>
#include <limits.h>

/* static state */
static seq_t *balance_cache = NULL;   /* array of found terms */
static size_t cache_cap = 0;          /* allocated capacity of the array */
static size_t cache_len = 0;          /* number of terms stored in the array */
static seq_t last_checked = 0;        /* last integer examined (wraps on overflow) */

/* enlarge the cache so that it can hold at least new_len elements */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap)
        return;               /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < new_len)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(balance_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* unrecoverable allocation failure */

    balance_cache = tmp;
    cache_cap = new_cap;
}

/* count the number of 1 bits in v */
static unsigned int popcount_seq(seq_t v)
{
    unsigned int c = 0;
    while (v) {
        c += (unsigned int)(v & 1);
        v >>= 1;
    }
    return c;
}

/* number of bits needed to represent v (floor(log2(v)) + 1) */
static unsigned int bitlen_seq(seq_t v)
{
    unsigned int l = 0;
    while (v) {
        v >>= 1;
        ++l;
    }
    return l;
}

/* return the n‑th term of A031443 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n < 1 */

    while (cache_len < (size_t)n) {
        /* increment candidate; wrap on overflow, which will stop the loop */
        seq_t cand = last_checked + 1;
        if (cand == last_checked)          /* overflow detected */
            break;
        last_checked = cand;

        /* skip candidates where the total number of bits is odd –
           balanced numbers require an even total length. */
        unsigned int bl = bitlen_seq(cand);
        if ((bl & 1u) != 0)
            continue;

        unsigned int pc = popcount_seq(cand);
        if (2u * pc == bl) {               /* balanced */
            grow_cache(cache_len + 1);
            balance_cache[cache_len++] = cand;
        }
    }
    return balance_cache[(size_t)n - 1];
}
