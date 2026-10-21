/*
 * A077667  Triangle read by rows. Let k = n-th composite number. The n-th row
 *          gives k smallest numbers unrelated to k (numbers which do not
 *          divide k, are not divisible by k and are not coprime to k).
 * https://oeis.org/A077667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   For each composite k (= the (2‑nd,3rd,4th,…) composite), output
 *            the k smallest positive integers m that are unrelated to k: 1)
 *            gcd(k,m) > 1, 2) k is not divisible by m, and 3) m is not
 *            divisible by k
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated row‑by‑row; a(n) is the n‑th element of that
   flattened triangle.  The implementation keeps a static cache so that
   arbitrary calls to a(n) are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/*----------------------------------------------------------------------
 * Basic helpers
 *----------------------------------------------------------------------*/

/* Trial division prime test – fast enough for the required range. */
static inline bool is_prime(uint64_t n)
{
    if (n < 2)        return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

static inline bool isComposite(uint64_t n)
{
    return n >= 4 && !is_prime(n);
}

/*----------------------------------------------------------------------
 * Cache for the sequence
 *----------------------------------------------------------------------*/
static seq_t   *seq_cache = NULL;   /* 1‑based: seq_cache[0] == a(1)   */
static size_t  seq_len   = 0;        /* number of terms stored          */
static size_t  seq_cap   = 0;        /* allocated capacity              */

/* Ensure the cache can hold at least target_len elements; grow geometrically. */
static void ensure_cache_capacity(size_t target_len)
{
    if (target_len <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < target_len)
        new_cap <<= 1;
    void *p = (seq_cache ? realloc(seq_cache, new_cap * sizeof(seq_t))
                         : malloc(new_cap * sizeof(seq_t)));
    seq_cache = (seq_t *)p;
    seq_cap = new_cap;
}

/*----------------------------------------------------------------------
 * Generate the triangle row by row until we have at least target_len terms.
 *--------------------------------------------------------------------*/
static void generate_until(size_t target_len)
{
    static uint64_t last_composite = 3;  /* 3 is not composite, so first is 4 */

    if (seq_len >= target_len)
        return;

    /* Produce additional rows as needed. */
    while (seq_len < target_len) {
        /* Find next composite number. */
        do {
            ++last_composite;
        } while (!isComposite(last_composite));

        uint64_t k = last_composite;          /* row length */
        uint64_t found = 0;                   /* numbers found in this row */

        /* Scan natural numbers until we have k unrelated numbers. */
        for (uint64_t m = 2; found < k; ++m) {
            if (gcd_u64(k, m) > 1 &&
                (k % m) != 0 &&
                (m % k) != 0) {
                ensure_cache_capacity(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)m;
                ++found;
            }
        }
    }
}

/*----------------------------------------------------------------------
 * Public sequence function
 *----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined – return 0 safely */
        return 0;

    generate_until((size_t)n);

    return seq_cache[n - 1];    /* cache is 1‑based */
}
