/*
 * A024775  Every prefix and suffix prime in base 7 (written in base 7).
 * https://oeis.org/A024775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates every base‑7 number whose every prefix and every suffix,
   interpreted in base 7, is a prime number.
   Numbers are emitted in increasing order and stored in a static cache.
   The algorithm builds numbers digit by digit, pruning prefixes that
   are not prime.  When a full number is constructed, all its suffixes
   are tested; if they are all prime the decimal value of the base‑7
   string is cached.  The cache is grown lazily so that any
   a(n) request, regardless of order, is answered in O(n).   */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *cache = NULL;             /* cached terms */
static size_t cache_len = 0;            /* number of cached terms   */
static size_t cache_cap = 0;            /* capacity of cache        */
static size_t next_len = 1;             /* next number of digits to generate */

/* Grow the cache if necessary and append a new item. */
static void cache_append(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
        if (!new_ptr) exit(EXIT_FAILURE);
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursively generate all base‑7 numbers of a fixed length.
   *digits holds the current digit sequence (0–6).
   prefix_val is the value of the current prefix (base 7).
   When the number is fully constructed, all suffixes are checked. */
static void generate_rec(int pos, const seq_t prefix_val, int *digits, int len) {
    if (pos == len) {
        /* All suffixes must be prime. */
        for (int start = 0; start < len; ++start) {
            seq_t suffix = 0;
            for (int j = start; j < len; ++j) {
                suffix = suffix * 7 + (seq_t)digits[j];
            }
            if (!is_prime_big(suffix))
                return;
        }
        /* Convert the base‑7 digit array to a decimal value. */
        seq_t val_dec = (seq_t)digits[0];
        for (int i = 1; i < len; ++i)
            val_dec = val_dec * 10 + (seq_t)digits[i];
        cache_append(val_dec);
        return;
    }

    for (int d = 0; d <= 6; ++d) {
        if (pos == 0 && d == 0)         /* no leading zero */
            continue;
        seq_t new_prefix = prefix_val * 7 + (seq_t)d;
        if (!is_prime_big(new_prefix))  /* prefix must be prime */
            continue;
        digits[pos] = d;
        generate_rec(pos + 1, new_prefix, digits, len);
    }
}

/* Generate all numbers of the given fixed length. */
static void generate_len(int len) {
    int digits[64];  /* len is expected to be modest (<= 64) */
    generate_rec(0, 0, digits, len);
}

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n) {
    if (cache_len >= (size_t)n)
        return;
    /* Generate numbers of increasing length until the cache is long enough. */
    while (cache_len < (size_t)n) {
        generate_len((int)next_len);
        ++next_len;
        /* To avoid infinite loops in a theoretical finite case, stop after
           a reasonable maximum length.  Adjust as needed. */
        if (next_len > 100)
            break;
    }
}

/* Public sequence function.  Returns the nth term (n >= 1).  */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;           /* offset is 1; undefined for n = 0 */

    ensure_cache(n);

    if ((size_t)n <= cache_len)
        return cache[n - 1];
    else
        return 0;           /* sequence terminates for this index (if finite) */
}
