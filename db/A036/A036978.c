/*
 * A036978  Numbers for which the "describe what you see" transform A045918
 *          produces a prime.
 * https://oeis.org/A036978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036978: Numbers for which the "describe what you see" transform
   (A045918) produces a prime.
   Computation: brute‑force enumeration of integers starting from 1,
   applying the transform and testing primality. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
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

/* Append the decimal representation of cnt to out, updating pos. */
static void append_count(int cnt, char *out, size_t *pos)
{
    char tmp[32];
    int tlen = 0;
    int v = cnt;
    if (v == 0) {
        tmp[tlen++] = '0';
    } else {
        while (v > 0) {
            tmp[tlen++] = '0' + (v % 10);
            v /= 10;
        }
        /* reverse */
        for (int i = 0, j = tlen - 1; i < j; ++i, --j) {
            char c = tmp[i];
            tmp[i] = tmp[j];
            tmp[j] = c;
        }
    }
    memcpy(out + *pos, tmp, tlen);
    *pos += tlen;
}

/* Apply the "describe‑what‑you‑see" transform to num and return the
   resulting value as seq_t. */
static seq_t describe_transform(seq_t num)
{
    /* Buffers large enough for all expected digits. */
    static char inbuf[4096];
    static char outbuf[8192];
    size_t inlen = sizeof(inbuf);
    seq_to_str(num, inbuf, &inlen);
    if (inlen >= sizeof(inbuf)) inlen = sizeof(inbuf) - 1;
    inbuf[inlen] = '\0';

    size_t outpos = 0;
    char current = inbuf[0];
    int count = 1;
    for (size_t i = 1; i < inlen; ++i) {
        char c = inbuf[i];
        if (c == current) {
            ++count;
        } else {
            append_count(count, outbuf, &outpos);
            outbuf[outpos++] = current;
            current = c;
            count = 1;
        }
    }
    /* final run */
    append_count(count, outbuf, &outpos);
    outbuf[outpos++] = current;
    outbuf[outpos] = '\0';

    return seq_from_str(outbuf);
}

/* Cache of found sequence values. */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;
static idx_t cache_cap   = 0;
static seq_t next_candidate = 1;

/* Return the n‑th term of A036978. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – a(1) is the first listed term */

    /* Fill the cache until we have n terms. */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        ++next_candidate;

        seq_t transformed = describe_transform(cand);
        if (is_prime_big(transformed)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
