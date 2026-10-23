/*
 * A041719  Denominators of continued fraction convergents to sqrt(379).
 * https://oeis.org/A041719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 25882394441081380*a(n-30) - a(n-60) for n>59. a(0)=1,
 *            a(1)=2, a(2)=15, a(3)=47, a(4)=109, a(5)=265, a(6)=1699,
 *            a(7)=20653, a(8)=22352, a(9)=110061, a(10)=132413, a(11)=242474,
 *            a(12)=374887, a(13)=1367135, a(14)=5843427, a(15)=112392248,
 *            a(16)=455412419, a(17)=1478629505, a(18)=1934041924,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Continued–fraction parameters for sqrt(379) */
static const uint32_t D  = 379U;          /* radicand */
static const uint32_t a0 = 19U;          /* floor(sqrt(379)) */

static uint32_t m      = 0U;
static uint32_t d_last = 1U;
static uint32_t a_curr = a0;             /* a current partial quotient */

/* 25882394441081380 equals the product of the 30 a_i in one era of the
   periodic continued fraction of sqrt(379).  */
static const seq_t RECIPROCAL_COEFF = (seq_t)25882394441081380ULL;

seq_t a(idx_t n) {
    /* Grow cache if necessary. */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms from cache_len up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term;

        if (i == 0) {
            term = (seq_t)1;
        } else if (i == 1) {
            /* a_curr now holds a1 after the first iteration. */
            term = (seq_t)a_curr * cache[i-1];
        } else if (i <= 59U) {
            term = (seq_t)a_curr * cache[i-1] + cache[i-2];
        } else {
            /* Linear recurrence for n > 59. */
            term = RECIPROCAL_COEFF * cache[i-30] - cache[i-60];
        }

        cache[i] = term;

        if (i <= 59U) {
            /* Advance continued‑fraction parameters for the next partial quotient. */
            uint32_t m_next = d_last * a_curr - m;
            uint32_t d_next = (D - m_next * m_next) / d_last;
            uint32_t a_next = (a0 + m_next) / d_next;
            m = m_next;
            d_last = d_next;
            a_curr = a_next;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
