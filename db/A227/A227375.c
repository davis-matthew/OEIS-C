/*
 * A227375  G.f.: 1/(1 - x*(1-x^6)/(1 - x^2*(1-x^7)/(1 - x^3*(1-x^8)/(1 -
 *          x^4*(1-x^9)/(1 - x^5*(1-x^10)/(1 - ...)))))), a continued
 *          fraction.
 * https://oeis.org/A227375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *   a(n) = a(n-1) + a(n-2) + a(n-3) - 2*a(n-6) - 2*a(n-7) - a(n-8)
 *          + a(n-10) + a(n-11) + a(n-12),  for n >= 12
 *
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5,
 *   a(6)=9, a(7)=14, a(8)=24, a(9)=41, a(10)=69, a(11)=118
 *
 * The function memoises results in a cache so it works for any
 * calling order.  It works for uint64_t indices and any seq_t type
 * specified by the build (fixed-size unsigned or arbitrary precision).
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;        /* cache of computed terms */
static idx_t  a_cache_len = 0;       /* number of terms stored */
static size_t a_cache_cap = 0;       /* capacity of a_cache */

/* Initial terms for n = 0 .. 11 */
static const seq_t a_init[12] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
    (seq_t)9, (seq_t)14,(seq_t)24,(seq_t)41,(seq_t)69,(seq_t)118
};

static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need > a_cache_cap) {
        size_t new_cap = need * 2;
        seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* Memory allocation failure: abort (no stdio allowed). */
            exit(1);
        }
        a_cache = new_ptr;
        a_cache_cap = new_cap;
    }
}

static void compute_range(idx_t from, idx_t to)
{
    for (idx_t i = from; i <= to; ++i) {
        seq_t val;
        if (i < 12) {
            val = a_init[(size_t)i];
        } else {
            val = a_cache[(size_t)(i - 1)];
            val += a_cache[(size_t)(i - 2)];
            val += a_cache[(size_t)(i - 3)];
            val -= a_cache[(size_t)(i - 6)] * (seq_t)2;
            val -= a_cache[(size_t)(i - 7)] * (seq_t)2;
            val -= a_cache[(size_t)(i - 8)];
            val += a_cache[(size_t)(i -10)];
            val += a_cache[(size_t)(i -11)];
            val += a_cache[(size_t)(i -12)];
        }
        a_cache[(size_t)i] = val;
    }
    a_cache_len = (size_t)(to + 1);
}

seq_t a(idx_t n)
{
    if (n >= a_cache_len) {
        ensure_capacity(n);
        compute_range(a_cache_len, n);
    }
    return a_cache[(size_t)n];
}
