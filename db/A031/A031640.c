/*
 * A031640  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 52.
 * https://oeis.org/A031640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that * the continued
 *            fraction of sqrt(k) has odd period and, * after deleting the
 *            last term of the periodic part, * the two central terms are both
 *            52
 * Range:     u8 none, u16 n <= 3, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Common headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* If compiled as C++17, include vector for dynamic array. */
#ifdef __cplusplus
#include <vector>
#endif

/* Integer square root (floor) for 64‑bit values.
 * Uses binary search with 128‑bit multiplication to avoid overflow. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (n >> 1) + 1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Return true iff k satisfies the required property. */
static bool satisfies(uint64_t k)
{
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* perfect square */

    uint64_t m = 0, d = 1, a = a0;
    /* Periodic part of continued fraction.  Size is kept small
       (period for k up to at least 10^12 < 20000). */
    int a_list[20000];
    int period = 0;

    while (true) {
        /* m = d*a - m */
        unsigned __int128 temp = (unsigned __int128)d * a;
        m = (uint64_t)(temp - m);

        /* d = (k - m*m) / d */
        unsigned __int128 num = (unsigned __int128)k - (unsigned __int128)m * m;
        d = (uint64_t)(num / d);

        a = (a0 + m) / d;          /* next term */
        if (period >= 20000) return false;   /* safety */
        a_list[period++] = (int)a;

        if (a == 2 * a0) break;    /* period completed */
    }

    if ((period & 1) == 0) return false;   /* period must be odd */

    /* Length after removing the last term */
    int len = period - 1;
    if ((len & 1) != 0) return false;      /* must be even */

    int mid = len >> 1;          /* mid = len/2, 0‑based */
    if (a_list[mid - 1] == 52 && a_list[mid] == 52)
        return true;
    return false;
}

/* Sequence access function.  Values are generated and memoised
 * on demand, so calls may arrive in any order. */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation: use std::vector for dynamic growth. */
    static std::vector<seq_t> buffer;
    static uint64_t last_checked = 1;   /* start checking from 2 */

    while (buffer.size() < (size_t)n) {
        uint64_t k = ++last_checked;
        if (satisfies(k))
            buffer.push_back((seq_t)k);
    }
    return buffer[(size_t)n - 1];
#else
    /* C implementation: use malloc/realloc for dynamic array. */
    static struct {
        seq_t *buf;      /* array of results */
        size_t cap;      /* allocated capacity */
        size_t size;     /* number of elements stored */
        uint64_t last_checked; /* last k examined */
    } *state = NULL;

    if (!state) {
        /* Initialise static state structure. */
        state = (void *)malloc(sizeof(*state));
        if (!state) return (seq_t)0;          /* out of memory */
        state->buf = NULL;
        state->cap = 0;
        state->size = 0;
        state->last_checked = 1;             /* start from 2 */
    }

    while (state->size < (size_t)n) {
        uint64_t k = ++state->last_checked;
        if (satisfies(k)) {
            /* Grow array if necessary. */
            if (state->size >= state->cap) {
                size_t newcap = state->cap ? state->cap * 2 : 16;
                seq_t *tmp = (void *)realloc(state->buf, newcap * sizeof(seq_t));
                if (!tmp) {                  /* allocation failed */
                    free(state->buf);
                    free(state);
                    return (seq_t)0;
                }
                state->buf = tmp;
                state->cap = newcap;
            }
            state->buf[state->size++] = (seq_t)k;
        }
    }

    return state->buf[(size_t)n - 1];
#endif
}
