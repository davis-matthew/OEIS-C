/*
 * A092629  Numbers that have a nonprime number of prime digits.
 * https://oeis.org/A092629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A092629: numbers that have a nonprime number of prime digits. */
/* The sequence starts 1,2,3,4,5,6,7,8,9,10,11,… where the 22nd term is 24. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for memoization */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t last_cand = 0;

/* enlarge cache to at least `need` elements */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2ULL : 128ULL;
    while (new_cap < need) {
        /* avoid overflow of new_cap */
        if (new_cap > (~(idx_t)0) / 2ULL) {
            new_cap = need;
            break;
        }
        new_cap *= 2ULL;
    }
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();
    cache = p;
    cache_cap = new_cap;
}

/* primality test for small integers (used only for digit counts) */
static bool is_small_prime(int x)
{
    if (x < 2)
        return false;
    for (int p = 2; p * p <= x; ++p)
        if (x % p == 0)
            return false;
    return true;
}

/* count the number of prime digits (2,3,5,7) in a non‑negative integer */
static int count_prime_digits(idx_t v)
{
    int cnt = 0;
    if (v == 0)
        return 0;          /* 0 has zero prime digits */
    while (v) {
        uint64_t d = v % 10ULL;
        if (d == 2ULL || d == 3ULL || d == 5ULL || d == 7ULL)
            ++cnt;
        v /= 10ULL;
    }
    return cnt;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n == 0, but defined to 0 */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        idx_t cand;
        for (cand = last_cand + 1; cache_size < n; ++cand) {
            int pd = count_prime_digits(cand);
            if (!is_small_prime(pd)) {     /* count is nonprime → keep the number */
                ensure_cache(cache_size + 1);
                cache[cache_size] = (seq_t)cand;
                ++cache_size;
            }
        }
        last_cand = cand - 1;               /* last number tested */
    }

    return cache[n - 1];
}
