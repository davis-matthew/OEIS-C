/*
 * A163759  Exactly three distinct primes occur as substrings of the digits of
 *          n.
 * https://oeis.org/A163759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer (n ≥ 1) that has exactly three distinct
 *            prime * substrings in its decimal representation. * * The
 *            function builds the sequence incrementally, caching results
 *            between * calls. It only relies on the utility functions
 *            supplied in the build * environment
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/num_digits.h"
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

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
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

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

typedef uint64_t idx_t; /* for compatibility with the build */

/* ---------- helper to manage the cached sequence ---------- */

static seq_t *cached_values = NULL;   /* pointer to array of values         */
static size_t cached_len   = 0;        /* number of values stored            */
static size_t cached_cap   = 0;        /* current allocated capacity         */

/* current number being tested for inclusion */
static seq_t cur_num = 0;
static bool cur_initialized = false;

/* expand cached array to at least 'needed' capacity */
static void expand_cache(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = (cached_cap == 0) ? 4 : cached_cap;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (cached_len > 0)
        mem_copy(new_arr, cached_values, cached_len * sizeof(seq_t));
    if (cached_values)
        free_seq_array(cached_values);
    cached_values = new_arr;
    cached_cap   = new_cap;
}

/* test if a number 'n' has exactly three distinct prime substrings */
static bool has_three_prime_substrings(seq_t n)
{
    size_t len = num_digits(n);
    char *buf = (char *)malloc(len + 1);
    if (!buf)
        return false; /* out of memory – unlikely in this context */
    seq_to_str(n, buf, &len);
    buf[len] = '\0';

    seq_t primes[3];
    size_t prime_count = 0;

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            unsigned int digit = (unsigned int)(buf[j] - '0');
            val = val * 10 + digit;
            if (val >= 2 && is_prime_big(val)) {
                bool already = false;
                for (size_t p = 0; p < prime_count; ++p)
                    if (primes[p] == val) {
                        already = true;
                        break;
                    }
                if (!already) {
                    if (prime_count == 3)
                        goto cleanup; /* more than three */
                    primes[prime_count++] = val;
                }
            }
        }
        /* early escape if already more than three */
        if (prime_count == 3)
            continue;
    }

    if (prime_count == 3) {
        free(buf);
        return true;
    }

cleanup:
    free(buf);
    return false;
}

/* ensure cached_values stores at least 'need' terms (need ≥ 1) */
static void ensure_cached(idx_t need)
{
    if (need <= 0)
        return;
    size_t target = (size_t)need;
    if (target <= cached_len)
        return;

    if (!cur_initialized) {
        cur_num = seq_from_str("2");
        cur_initialized = true;
    }

    while (cached_len < target) {
        if (has_three_prime_substrings(cur_num)) {
            // add to cache
            expand_cache(cached_len + 1);
            cached_values[cached_len++] = cur_num;
        }
        ++cur_num; /* test next number */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard against zero */
        return seq_from_str("0");

    ensure_cached(n);
    return cached_values[n - 1];
}
