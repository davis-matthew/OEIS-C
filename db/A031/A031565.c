/*
 * A031565  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 67.
 * https://oeis.org/A031565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for k such that the continued fraction
   for sqrt(k) has even period and, after deleting the last term, the
   central term of the remaining periodic part is 67.  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- local helpers ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0, bit = 1ULL << 62;      /* highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Test whether a given k satisfies the continued‑fraction property. */
static bool good(uint64_t k)
{
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* perfect square */

    uint64_t m = 0, d = 1, a = a0;
    static uint64_t terms[16384];            /* store terms of period */
    size_t sz = 0;

    while (true) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;

        if (sz >= sizeof(terms)/sizeof(terms[0])) return false;  /* safety */
        terms[sz++] = a;

        if (a == 2 * a0) break;               /* period finished */
    }

    if (sz % 2 != 0) return false;                 /* odd period -> reject */

    /* Central term after deleting the last term */
    return terms[(sz - 2) / 2] == 67;
}

/* ---------- caching infrastructure ---------- */
static seq_t   *cache  = NULL;                /* cached sequence values   */
static size_t   cap    = 0;                   /* allocated capacity       */
static size_t   len    = 0;                   /* number of cached terms   */
static uint64_t nextk  = 1;                   /* next integer to test    */

static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t newcap = cap ? cap * 2 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure */
    cache = tmp;
    cap = newcap;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* n >= 1; behavior for n==0 is irrelevant to OEIS */
    while (len < n) {
        for (;;) {
            uint64_t k = nextk++;
            if (good(k)) {
                ensure_capacity(len + 1);
                cache[len++] = (seq_t)k;
                break;
            }
        }
    }
    return cache[n - 1];
}
