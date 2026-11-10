/*
 * A236697  First differences of A131741.
 * https://oeis.org/A236697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A131741(n+1) - A131741(n); A131741 is built greedily
 *            (smallest unused prime that forms no 3-term AP with the earlier
 *            terms), using a growing prime sieve and a bitset of forbidden
 *            values 2*p - q
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9609 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---- growing sieve: A131741_comp[i] != 0 iff i is composite (i <= lim) ---- */
static uint8_t *A131741_comp = NULL;
static uint64_t A131741_sieve_lim = 0;

static void A131741_sieve_to(uint64_t need)
{
    if (need <= A131741_sieve_lim) return;
    uint64_t lim = A131741_sieve_lim ? A131741_sieve_lim : 1024;
    while (lim < need) lim <<= 1;
    uint8_t *c = (uint8_t *)realloc(A131741_comp, (size_t)(lim + 1));
    if (!c) abort();
    memset(c, 0, (size_t)(lim + 1));
    c[0] = 1; c[1] = 1;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (!c[i])
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
    A131741_comp = c;
    A131741_sieve_lim = lim;
}

/* ---- forbidden-value bitset ---- */
static uint64_t *A131741_forb = NULL;
static uint64_t A131741_forb_words = 0;

static void A131741_forb_ensure(uint64_t pos)
{
    uint64_t need = (pos >> 6) + 1;
    if (need <= A131741_forb_words) return;
    uint64_t w = A131741_forb_words ? A131741_forb_words : 1024;
    while (w < need) w <<= 1;
    uint64_t *b = (uint64_t *)realloc(A131741_forb, (size_t)w * sizeof(uint64_t));
    if (!b) abort();
    for (uint64_t i = A131741_forb_words; i < w; i++) b[i] = 0;
    A131741_forb = b;
    A131741_forb_words = w;
}

static bool A131741_is_forb(uint64_t pos)
{
    if ((pos >> 6) >= A131741_forb_words) return false;
    return (A131741_forb[pos >> 6] >> (pos & 63)) & 1ULL;
}

/* ---- A131741 terms (native 64-bit) ---- */
static uint64_t *A131741_t = NULL;
static size_t A131741_len = 0, A131741_cap = 0;
static uint64_t A131741_next = 3;

static uint64_t A131741_term(size_t n) /* 1-based */
{
    if (A131741_len == 0) {
        A131741_cap = 1024;
        A131741_t = (uint64_t *)malloc(A131741_cap * sizeof(uint64_t));
        if (!A131741_t) abort();
        A131741_t[0] = 2;
        A131741_len = 1;
    }
    while (A131741_len < n) {
        uint64_t c = A131741_next;
        A131741_next += 2;
        if (A131741_is_forb(c)) continue;
        A131741_sieve_to(c);
        if (A131741_comp[c]) continue;

        if (A131741_len == A131741_cap) {
            A131741_cap <<= 1;
            uint64_t *nt = (uint64_t *)realloc(A131741_t, A131741_cap * sizeof(uint64_t));
            if (!nt) abort();
            A131741_t = nt;
        }
        A131741_forb_ensure(2 * c);
        uint64_t *fb = A131741_forb;
        uint64_t c2 = 2 * c;
        for (size_t i = 0; i < A131741_len; i++) {
            uint64_t f = c2 - A131741_t[i];
            fb[f >> 6] |= 1ULL << (f & 63);
        }
        A131741_t[A131741_len++] = c;
    }
    return A131741_t[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t hi = A131741_term((size_t)n + 1);
    uint64_t lo = A131741_term((size_t)n);
    return (seq_t)(hi - lo);
}
