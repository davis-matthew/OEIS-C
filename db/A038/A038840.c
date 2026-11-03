/*
 * A038840  Cubes that are concatenations of primes.
 * https://oeis.org/A038840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function generates the sequence of cubes that can be
 *            concatenated into a sequence of prime numbers. The n-th term
 *            (a(1) = 27) is returned as seq_t. The implementation checks
 *            cubes of consecutive integers for the concatenation property
 *            using a dynamic programming splitter. The sequence is sto...
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- utility headers ---------- */
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "../utils/ipow_idx.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* ---------- static cache ---------- */
static seq_t *cache = NULL;          /* array of found cubes */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_size = 0;        /* number of stored cubes */
static idx_t current_base = 0;       /* largest integer whose cube has been examined */

/* ---- helper: resize the cache array ---- */
static void resize_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (!new_arr) {
        abort(); /* out of memory */
    }
    for (size_t i = 0; i < cache_size; ++i) {
        new_arr[i] = cache[i];
    }
    if (cache) {
        seq_free_array(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---- helper: test whether a decimal cube can be split into primes ---- */
static bool cube_is_concatenation(seq_t cube)
{
    /* Convert cube to decimal string */
    int digit_count = num_digits(cube);          /* number of decimal digits */
    if (digit_count <= 0) return false;         /* safety */
    size_t bufsize = (size_t)digit_count + 1;
    char *buf = (char *)malloc(bufsize);
    if (!buf) abort();
    size_t actual_len;
    seq_to_str(cube, buf, &actual_len);
    buf[actual_len] = '\0';

    /* dynamic programming: dp[pos] == true if suffix starting at pos can be split */
    bool *dp = (bool *)malloc((actual_len + 1) * sizeof(bool));
    if (!dp) {
        free(buf);
        abort();
    }
    dp[actual_len] = true;                      /* empty suffix is trivially split */

    for (int pos = actual_len - 1; pos >= 0; --pos) {
        if (buf[pos] == '0') {                  /* leading zero is not allowed */
            dp[pos] = false;
            continue;
        }
        seq_t val = 0;
        bool found = false;
        for (int i = pos; i < actual_len; ++i) {
            int digit = buf[i] - '0';
            val = val * 10 + digit;
            if (val < 2) continue;              /* not prime */
            if (is_prime_big(val) && dp[i + 1]) {
                found = true;
                break;
            }
        }
        dp[pos] = found;
    }

    bool result = dp[0];
    free(dp);
    free(buf);
    return result;
}

/* ============================================================= */
/* Public function required by the test harness  */
seq_t a(idx_t n)          /* n >= 1 */
{
    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        ++current_base;                         /* try next integer */
        seq_t cube = ipow_idx((seq_t)current_base, 3);
        if (cube_is_concatenation(cube)) {
            if (cache_size == cache_cap) {
                resize_cache();
            }
            cache[cache_size++] = cube;
        }
    }
    return cache[n - 1];
}
