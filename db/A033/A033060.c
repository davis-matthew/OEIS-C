/*
 * A033060  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A033060 – Numbers whose base‑9 representation S=Σ d(i)·9^i has the property
 * that for every odd i (1,3,5,…) the digit d(m‑i) is odd, where m is the index
 * of the most significant digit.  (Equivalently, digits whose position
 * parity differs from that of m must be odd.)
 *
 * The function computes the n‑th number in this sequence by generating the
 * conditionally valid integers in increasing order.
 */
#include <stdlib.h>
#include <stdbool.h>

/* static cache for previously computed terms */
static seq_t *cache = NULL;   /* array of a(k) for k = 1 .. cache_len */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of the array */
static seq_t next_candidate = 1;   /* number to test next */

/* test whether x satisfies the base‑9 digit condition */
static bool valid_base9(seq_t x)
{
    /* collect digits of x in base 9 */
    unsigned int digits[32];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (unsigned int)(tmp % 9);
        tmp /= 9;
    }
    if (len == 0) return false;          /* x == 0 is not used */

    /* m = highest index = len-1 */
    size_t m_parity = (len - 1) & 1U;

    for (size_t j = 0; j < len; ++j) {
        if ( (m_parity ^ (j & 1U)) && ((digits[j] & 1U) == 0) )
            return false;               /* required digit is even */
    }
    return true;
}

/* ensure cache has capacity for at least need entries */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < need) new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* main entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* out of bounds; not used by tests */

    /* generate terms until we have at least n */
    while (cache_len < (size_t)n) {
        if (valid_base9(next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;     /* test the next integer */
    }
    return cache[n-1];
}
