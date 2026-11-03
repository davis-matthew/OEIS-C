/*
 * A050656  Numbers n such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 5.
 * https://oeis.org/A050656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050656 by scanning integers and counting primes
   produced by replacing one decimal digit by any other digit
   (excluding replacements that create a leading zero).  A number
   qualifies when it yields exactly five distinct primes.  The
   sequence is cached for repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* Static cache of found terms */
static seq_t *term_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Ensure cache can hold at least 'needed' terms */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;
    term_cache = mem_resize(seq_t, term_cache, new_cap);
    cache_cap = new_cap;
}

/* Count how many distinct primes are produced by changing a
   single decimal digit of n to any other digit, respecting
   the “no leading zero” rule. */
static bool has_five_primes(seq_t n) {
    uint8_t digits[64];
    size_t len;
    get_digits(n, digits, &len);
    if (len == 0)
        return false;

    /* Precompute positional weights */
    seq_t weights[64];
    seq_t w = 1;
    for (size_t i = 0; i < len; ++i) {
        weights[i] = w;
        w = w * 10;
    }

    seq_t uniq[10];   /* hold seen primes; we store at most 5 */
    int uniq_cnt = 0;

    for (size_t pos = 0; pos < len && uniq_cnt <= 5; ++pos) {
        int old_d = digits[pos];
        for (int d = 0; d <= 9; ++d) {
            if (d == old_d)
                continue;
            /* Skip replacements that turn the most‑significant digit
               into zero (leading zero). */
            if (pos == len - 1 && d == 0)
                continue;

            int diff = d - old_d;
            seq_t new_val = n;
            if (diff > 0)
                new_val = new_val + (seq_t)diff * weights[pos];
            else
                new_val = new_val - (seq_t)(-diff) * weights[pos];

            if (!is_prime_big(new_val))
                continue;

            /* Skip duplicates */
            bool seen = false;
            for (int k = 0; k < uniq_cnt; ++k) {
                if (uniq[k] == new_val) {
                    seen = true;
                    break;
                }
            }
            if (seen)
                continue;

            uniq[uniq_cnt++] = new_val;
            if (uniq_cnt > 5)
                return false;   /* too many primes */
        }
    }
    return uniq_cnt == 5;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for offset 1 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t candidate = (cache_len == 0) ? 1
                                          : term_cache[cache_len - 1] + 1;

        /* Find the next qualifying candidate */
        for (;;) {
            if (has_five_primes(candidate)) {
                ensure_cache_capacity(cache_len + 1);
                term_cache[cache_len++] = candidate;
                break;
            }
            candidate = candidate + 1;
        }
    }

    return term_cache[n - 1];   /* 1‑based offset */
}
