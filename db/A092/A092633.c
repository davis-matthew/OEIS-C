/*
 * A092633  Nonprime numbers with exactly three nonprime digits.
 * https://oeis.org/A092633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th nonprime number whose decimal representation
 *            contains exactly three nonprime digits (nonprime digits are
 *            0,1,4,6,8,9)
 * Range:     u8 n <= 30, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_seq_array, get_digits, is_prime_big, seq_free_array,
 *            seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"        /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */
#include "../utils/seq_from_str.h"      /* seq_t seq_from_str(const char *s); */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/seq_realloc.h"       /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */
#include "../utils/seq_free_array.h"    /* void seq_free_array(seq_t *p); */

/* Cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;           /* current capacity */
static size_t cache_len = 0;           /* number of terms stored */

/* Candidate to start searching from */
static seq_t next_candidate;

/* Flag to run one‑time initialization */
static bool initialized = false;

/* Helper: count the number of nonprime decimal digits in a value */
static int count_nonprime_digits(seq_t n)
{
    uint8_t buf[128];
    size_t len;
    get_digits(n, buf, &len);          /* write digits into buf; order is irrelevant */
    int cnt = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (d == 0 || d == 1 || d == 4 || d == 6 || d == 8 || d == 9)
            ++cnt;
    }
    return cnt;
}

/* Helper: make sure cache and next_candidate are initialized */
static void ensure_initialized(void)
{
    if (initialized)
        return;
    /* start with a capacity of 128 terms */
    cache_cap = 128;
    cache = alloc_seq_array(cache_cap);
    /* first term according to OEIS definition */
    next_candidate = seq_from_str("100");
    initialized = true;
}

/* Helper: increase cache size to accommodate at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 128;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, but harmless */
    ensure_initialized();

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t val = next_candidate;
        while (true) {
            if (!is_prime_big(val) && count_nonprime_digits(val) == 3)
                break;              /* found the next term */
            val = val + 1;          /* increment */
        }

        grow_cache(cache_len + 1);   /* ensure room for one more */
        cache[cache_len++] = val;
        next_candidate = val + 1;    /* next search starts beyond this number */
    }

    return cache[n - 1];
}
