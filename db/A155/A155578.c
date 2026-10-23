/*
 * A155578  Intersection of A000404 and A155717: N = a^2 + b^2 = c^2 + 7*d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intersection of A000404 and A155717:
   A(n) is the nth positive integer that can be written as
   a^2 + b^2 = c^2 + 7*d^2 with a,b,c,d ≥ 1. */
/* The implementation uses a simple sieving approach:
   1. For a growing limit L, all values a^2+b^2 with a,b ≥ 1 and ≤ L
      are marked in a boolean array.
   2. All values c^2+7*d^2 with c,d ≥ 1 and ≤ L are marked in a second
      boolean array.
   3. Numbers present in both arrays are collected into a dynamic
      sequence.  The array is extended until it contains at least n terms. */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* integer square‑root for idx_t (assumed unsigned 64‑bit)              */
/* --------------------------------------------------------------------- */
static inline idx_t _isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* --------------------------------------------------------------------- */
/* static data structure for the sequence                               */
/* --------------------------------------------------------------------- */
static seq_t *seq      = NULL;   /* dynamic array of discovered terms   */
static size_t seq_size = 0;      /* number of stored terms              */
static size_t seq_cap  = 0;      /* allocated capacity                  */
static idx_t  cur_lim  = 0;      /* highest value already processed     */

/* --------------------------------------------------------------------- */
/* generate all new intersection numbers in the range (old_lim, new_lim]*/
/* --------------------------------------------------------------------- */
static void _extend_to(idx_t target_n)
{
    /* initial allocation if needed */
    if (!seq_cap) {
        seq_cap = 1024;
        seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }

    /* choose an initial and subsequent limits */
    idx_t limit = cur_lim? cur_lim * 2 + 1000 : 100000;

    while (seq_size < target_n) {
        /* allocate and zero a two‑bit mask array up to 'limit' */
        size_t sz = (size_t)(limit + 1);
        uint8_t *flag_two  = (uint8_t*)calloc(sz, sizeof(uint8_t));
        uint8_t *flag_seven = (uint8_t*)calloc(sz, sizeof(uint8_t));
        if (!flag_two || !flag_seven) abort();

        /* mark numbers that are sums of two positive squares */
        for (idx_t a = 1; (idx_t)a * a <= limit; ++a) {
            idx_t a_sq = a * a;
            idx_t max_b = _isqrt(limit - a_sq);
            for (idx_t b = a; b <= max_b; ++b) {
                flag_two[a_sq + b * b] = 1;
            }
        }

        /* mark numbers of the form c^2 + 7*d^2 with positive d */
        for (idx_t d = 1; (idx_t)7 * d * d <= limit; ++d) {
            idx_t base = (idx_t)7 * d * d;
            idx_t max_c = _isqrt(limit - base);
            for (idx_t c = 1; c <= max_c; ++c) {
                flag_seven[base + c * c] = 1;
            }
        }

        /* collect intersection numbers that are larger than the previous limit */
        for (idx_t x = cur_lim + 1; x <= limit; ++x) {
            if (flag_two[x] && flag_seven[x]) {
                if (seq_size == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) abort();
                }
                seq[seq_size++] = (seq_t)x;
            }
        }

        free(flag_two);
        free(flag_seven);

        cur_lim = limit;
        limit = limit * 2 + 1000;   /* next expansion */
    }
}

/* --------------------------------------------------------------------- */
/* public function required by the build system                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but defensive */
    _extend_to(n);                          /* ensure at least n terms  */
    return seq[n - 1];
}
