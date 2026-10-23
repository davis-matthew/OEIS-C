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
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 27, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_len, is_prime_big, seq_alloc_new_array, seq_free_array,
 *            seq_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- utility headers ---------- */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */
#include "../utils/seq_to_str.h"     /* seq_t seq_to_str(seq_t x, char*buf, size_t*len); */
#include "../utils/seq_pow.h"        /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x); */
#include "../utils/seq_alloc_new_array.h" /* seq_t* seq_alloc_new_array(size_t n); */
#include "../utils/seq_free_array.h"      /* void seq_free_array(seq_t* p); */

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
    int digit_count = digit_len(cube);          /* number of decimal digits */
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
        seq_t cube = seq_pow((seq_t)current_base, 3);
        if (cube_is_concatenation(cube)) {
            if (cache_size == cache_cap) {
                resize_cache();
            }
            cache[cache_size++] = cube;
        }
    }
    return cache[n - 1];
}
