/*
 * A040717  Continued fraction for sqrt(745).
 * https://oeis.org/A040717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth partial quotient in the simple continued
 *            fraction expansion of √745. The sequence starts with a(0)=27
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants for √745 */
    const uint64_t D   = 745ULL;
    const uint64_t a0  = 27ULL;          /* floor(sqrt(745)) */

    /* Static state to be shared between calls */
    static seq_t *cache   = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;
    static uint64_t m = 0;
    static uint64_t d = 1;
    static uint64_t cur_a = a0;
    static bool initialized = false;

    /* Helper to grow the cache */
    if (n >= cached_cap) {
        size_t new_cap = cached_cap > 0 ? cached_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Initialization: compute first term a(0) */
    if (!initialized) {
        cache[0] = (seq_t)a0;
        cached_len = 1;
        initialized = true;
    }

    /* Generate terms until we have up to n */
    while (cached_len <= (size_t)n) {
        /* Standard recurrence for continued fractions of √D */
        uint64_t m_next = d * cur_a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Update state */
        m = m_next;
        d = d_next;
        cur_a = a_next;

        cache[cached_len] = (seq_t)a_next;
        ++cached_len;
    }

    return cache[(size_t)n];
}
