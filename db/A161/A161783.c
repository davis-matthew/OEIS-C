/*
 * A161783  Squares n^2 whose decimal expansion contains n as a substring.
 * https://oeis.org/A161783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A161783 – squares m^2 whose decimal representation contains m
    as a contiguous substring.  The function builds the sequence on
    demand, caching results so subsequent calls are fast.  It works for
    any seq_t/idx_t pair supported by the build.  The implementation
    uses only integer arithmetic and standard library headers. */

/* ---------- necessary includes ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- helper: convert a seq_t number to its decimal digits
              (most‑significant first) ---------- */
static void seq_to_digits(seq_t x, uint8_t buf[], size_t *len)
{
    if (x == (seq_t)0) {
        buf[0] = 0;
        *len   = 1;
        return;
    }
    size_t pos = 0;
    seq_t tmp  = x;
    while (tmp > (seq_t)0) {
        buf[pos++] = (uint8_t)(tmp % (seq_t)10);
        tmp        = tmp / (seq_t)10;
    }
    /* reverse to most‑significant first */
    for (size_t i = 0; i < pos/2; ++i) {
        uint8_t t   = buf[i];
        buf[i]      = buf[pos-1-i];
        buf[pos-1-i] = t;
    }
    *len = pos;
}

/* ---------- helper: check whether decimal representation of sq
               contains decimal representation of root ---------- */
static bool contains_root(seq_t sq, idx_t root)
{
    uint8_t buf_sq[128], buf_root[64];
    size_t len_sq, len_root;
    seq_to_digits(sq, buf_sq, &len_sq);
    seq_to_digits((seq_t)root, buf_root, &len_root);
    if (len_root > len_sq) return false;
    for (size_t i = 0; i + len_root <= len_sq; ++i) {
        bool ok = true;
        for (size_t j = 0; j < len_root; ++j) {
            if (buf_sq[i+j] != buf_root[j]) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

/* ---------- dynamic cache helper ---------- */
static void cache_append(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*cap == 0) {
        *cap = 8;
        *cache = (seq_t *)malloc(*cap * sizeof(seq_t));
    } else if (*len >= *cap) {
        *cap = (*cap << 1) + 1;
        *cache = (seq_t *)realloc(*cache, *cap * sizeof(seq_t));
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* static cache and generator state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_root = 1;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t r = (seq_t)next_root;
        seq_t sq = r * r;                     /* n^2 */
        if (contains_root(sq, next_root))
            cache_append(&cache, &cache_len, &cache_cap, sq);
        next_root++;
    }

    return cache[n-1];
}
