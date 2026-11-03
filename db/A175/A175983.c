/*
 * A175983  Squares n^2 that become prime after omitting all ones in their
 *          decimal expansion.
 * https://oeis.org/A175983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A175983: squares n^2 such that removing all decimal digits '1'
 * from n^2 leaves a prime. The function returns the nth such square
 * (a(1) = 121, a(2) = 1089, …).  Memoization is used so that repeated
 * calls are fast and order-independent.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *cache = NULL;   /* memoized terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_size = 0; /* number of cached terms */
static idx_t next_k = 1;      /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 – behaviour before it is irrelevant */

    if (n <= cache_size) return cache[n - 1];

    size_t found = cache_size;   /* count of cached qualifying squares */

    for (idx_t k = next_k; ; ++k) {
        seq_t square = (seq_t)k * k;

        /* obtain decimal representation of square */
        char buf[128];
        size_t len;
        seq_to_str(square, buf, &len);

        /* build number after deleting all '1' digits */
        seq_t trimmed = 0;
        for (size_t i = 0; i < len; ++i) {
            char c = buf[i];
            if (c == '1') continue;
            trimmed = trimmed * 10 + (seq_t)(c - '0');
        }

        if (trimmed > 1 && is_prime_big(trimmed)) {
            /* cache the term */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;        /* allocation failure fallback */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = square;
            ++found;
            if (found == n) {
                next_k = k + 1;
                return square;
            }
        }
    }
}
