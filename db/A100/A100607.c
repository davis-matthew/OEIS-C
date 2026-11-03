/*
 * A100607  Concatenated primes of order 3.
 * https://oeis.org/A100607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9222 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over primes in increasing order and keep those
   that can be represented as concatenations of exactly three primes.
   For each prime, the function checks all splits of its decimal
   representation into three parts, verifies that each part is a prime
   (no leading zeros allowed), and if found, stores the prime in a
   static array.  The function supports out‑of‑order requests for n
   by caching all previously found terms.  It should be fast enough
   for n up to ~10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_seq.h"
#include "../utils/mem.h"

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

/* Parse a decimal substring buf[start..end) into a seq_t value. */
static seq_t parse_substring(const char *buf, idx_t start, idx_t end)
{
    seq_t val = 0;
    for (idx_t i = start; i < end; ++i) {
        val = val * 10 + (seq_t)(buf[i] - '0');
    }
    return val;
}

static seq_t *cached_vals = NULL;      /* array of found terms */
static idx_t  cached_count = 0;        /* number of stored terms */
static idx_t  cached_capacity = 0;     /* allocated capacity */
static seq_t  next_candidate = 2;      /* next integer to test */

/* Return true if prime p can be split into three primes (no leading zeros). */
static bool is_concat_of_three_primes(seq_t p)
{
    /* Convert p to decimal string. */
    idx_t len = num_digits_seq(p);
    /* Allocate buffer exactly large enough. */
    char *s = (char *)malloc((size_t)len + 1);
    if (!s) return false;          /* safety */
    seq_to_dec_string(p, s, (size_t)len + 1);

    /* Try all splits i<j where 1≤i<j<len. */
    for (idx_t i = 1; i < len - 1; ++i) {
        if (s[i] == '0') continue;            /* avoid leading zero in second part */
        seq_t a = parse_substring(s, 0, i);
        if (!is_prime_seq(a)) continue;
        for (idx_t j = i + 1; j < len; ++j) {
            if (s[j] == '0') continue;        /* avoid leading zero in third part */
            seq_t b = parse_substring(s, i, j);
            if (!is_prime_seq(b)) continue;
            seq_t c = parse_substring(s, j, len);
            if (!is_prime_seq(c)) continue;
            free(s);
            return true;                      /* found a valid split */
        }
    }
    free(s);
    return false;                              /* no valid split found */
}

/* Append a value to the cached array, resizing if necessary. */
static void cached_append(seq_t val)
{
    if (cached_count == cached_capacity) {
        idx_t new_cap = (cached_capacity == 0) ? 64 : cached_capacity * 2;
        seq_t *new_arr = mem_resize(seq_t, cached_vals, (size_t)new_cap);
        if (!new_arr) return;                 /* safety: allocation failed */
        cached_vals = new_arr;
        cached_capacity = new_cap;
    }
    cached_vals[cached_count++] = val;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Build terms until we have at least n terms cached. */
    while (cached_count < n) {
        /* Test if next_candidate is prime. */
        if (is_prime_seq(next_candidate)) {
            if (is_concat_of_three_primes(next_candidate)) {
                cached_append(next_candidate);
            }
        }
        ++next_candidate;                     /* move to next integer */
    }
    return cached_vals[n - 1];
}
