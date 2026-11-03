/*
 * A182180  Semiprimes that become prime when their digits are sorted into
 *          nonincreasing order.
 * https://oeis.org/A182180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth semiprime that becomes prime when its decimal
 *            digits are sorted in non‑increasing order
 * Range:     u8 n <= 5, u16 n <= 467, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 949 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
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

/* Cache for already found terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Next integer to test */
static seq_t next_candidate = 2;

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap ? cache_cap * 2 : 16);
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_len++] = val;
}

/* Convert a number to a string and return a copy of the digits
   sorted in non‑increasing order as a new number. */
static seq_t sort_digits_desc(seq_t x)
{
    /* Allocate a buffer large enough for the decimal representation. */
    size_t buf_len = (size_t)num_digits(x) + 1;
    char *buf = (char *)malloc(buf_len);
    if (!buf) return 0; /* should not happen */

    size_t len = 0;
    seq_to_str(x, buf, &len);          /* buf now contains the decimal string */
    /* Convert to an array of digits. */
    uint8_t *dig = (uint8_t *)malloc(len);
    if (!dig) { free(buf); return 0; }

    for (size_t i = 0; i < len; ++i)
        dig[i] = (uint8_t)(buf[i] - '0');

    /* Simple insertion sort in descending order. */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = dig[i];
        size_t j = i;
        while (j > 0 && dig[j - 1] < key) {
            dig[j] = dig[j - 1];
            --j;
        }
        dig[j] = key;
    }

    /* Build the sorted number from the digit array. */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i)
        res = res * 10 + dig[i];

    free(dig);
    free(buf);
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 */

    /* Generate terms until the cache holds at least n terms. */
    while (cache_len < n) {
        /* Test current candidate. */
        if (bigomega_seq(next_candidate) == (seq_t)2) {
            seq_t sorted = sort_digits_desc(next_candidate);
            if (is_prime_big(sorted))
                cache_append(next_candidate);
        }
        /* Increment to the next integer. */
        ++next_candidate;
    }

    return cache[n - 1];
}
