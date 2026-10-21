/*
 * A065336  a(0) = 1; thereafter, a(n) is the smallest number such that Sum_{m
 *          = 0 .. n-1} a(m)*a(m+1) is a square.
 * https://oeis.org/A065336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A065336: a(0) = 1; for n>0 a(n) is the smallest positive integer x such that
 *  S(n) = Σ_{m=0}^{n-1} a(m)*a(m+1) + a(n-1)*x is a perfect square.
 *  Here S(0)=0.
 *
 * The implementation memoises all previously computed terms in a static
 * container (std::vector for C++ and a malloc'ed array for C).  The
 * running sum is stored in an intermediate type that is capable of
 * representing numbers larger than seq_t when seq_t is a builtin
 * integer type.  For arbitrary‑precision seq_t the sum itself is of
 * type seq_t.
 *
 *   * The function works for any idx_t (uint64_t) and any seq_t
 *     provided by the build.  For C++ seq_t may be a big integer
 *     class; for C seq_t is an unsigned integer type.
 *
 *   * The search for the next value proceeds by trying successive
 *     integers t ≥ floor_sqrt(S) until (t² – S) is a positive
 *     multiple of a(n‑1).  The quotient gives a(n).
 *
 *   * The implementation uses only integer arithmetic and relies
 *     on the supplied utility floor_sqrt.
 */
 
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
#include <vector>
/* C++ implementation ----------------------------------------------------- */
static std::vector<seq_t> a_vals;
static seq_t sum;
static bool init = false;

static void ensure_init(void)
{
    if (!init) {
        a_vals.reserve(32);
        a_vals.push_back((seq_t)1);   // a(0)
        sum = (seq_t)0;
        init = true;
    }
}

static void compute_up_to(uint64_t n)
{
    ensure_init();
    if (a_vals.size() > n) return;
    for (uint64_t idx = a_vals.size(); idx <= n; ++idx) {
        seq_t prev = a_vals[idx - 1];
        seq_t t = floor_sqrt(sum);
        for (;;) {
            seq_t t2 = t * t;
            if (t2 < sum) { t++; continue; }
            seq_t diff = t2 - sum;
            if (diff && (diff % prev == 0)) {
                seq_t x = diff / prev;
                if (x >= (seq_t)1) {
                    a_vals.push_back(x);
                    sum += prev * x;
                    break;
                }
            }
            ++t;
        }
    }
}

seq_t a(idx_t n)
{
    compute_up_to((uint64_t)n);
    return a_vals[(size_t)n];
}

#else
/* C implementation -------------------------------------------------------- */
static seq_t *a_vals = NULL;
static size_t cap = 0;   /* capacity of a_vals */
static size_t len = 0;   /* number of stored terms */
static __uint128_t sum = 0;

static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_vals = tmp;
    cap = new_cap;
}

static unsigned long long isqrt_uint128(__uint128_t x)
{
    unsigned long long low = 0, high = (unsigned long long)-1;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

static void compute_up_to(idx_t n)
{
    ensure_capacity((size_t)n + 1);
    if (len == 0) {            /* initial state */
        a_vals[0] = (seq_t)1;  /* a(0) */
        sum = 0;
        len = 1;
    }
    for (size_t idx = len; idx <= (size_t)n; ++idx) {
        seq_t prev = a_vals[idx - 1];
        unsigned long long t = isqrt_uint128(sum);
        for (;;) {
            __uint128_t t2 = (__uint128_t)t * t;
            if (t2 < sum) { t++; continue; }
            __uint128_t diff = t2 - sum;
            if (diff && (diff % prev == 0)) {
                __uint128_t x = diff / prev;
                if (x >= 1) {
                    a_vals[idx] = (seq_t)x;
                    sum += (__uint128_t)prev * x;
                    break;
                }
            }
            ++t;
        }
    }
    len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_vals[(size_t)n];
}
#endif
