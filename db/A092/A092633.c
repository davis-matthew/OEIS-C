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
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/get_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

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
