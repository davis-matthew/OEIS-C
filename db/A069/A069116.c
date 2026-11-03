/*
 * A069116  Composite k such that the squarefree part of k is greater than
 *          sqrt(k).
 * https://oeis.org/A069116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9897 (2026-11-03)
 * Helpers:   ipow_i32, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A069116: Composite k with squarefree part greater than √k
 * a(1)=6, a(2)=10, a(3)=14, ...
 * Squarefree part is the product of primes with odd exponent in k.
 * The formula is not simply a closed form; we generate terms by
 * testing successive composite integers and collecting those that satisfy
 * the condition.  The results are cached so that calls with 
 * arbitrary n order remain efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* cache of sequence values */
    static seq_t *vals   = NULL;
    static size_t vals_cap = 0;
    static size_t vals_len = 0;

    /* last composite examined */
    static idx_t last_checked = 0;

    /* grow cache until we have at least n terms */
    while (vals_len < (size_t)n) {
        ++last_checked;                     /* examine next integer */
        uint64_t k = (uint64_t)last_checked;

        uint64_t p = smallest_prime_factor(k);
        if (p == k)                 /* k is prime; skip */
            continue;

        /* compute squarefree part (kernel) of k */
        seq_t kernel = 1;
        uint64_t temp = k;
        while (temp > 1) {
            uint64_t pr = smallest_prime_factor(temp);
            int exp = 0;
            while (temp % pr == 0) {
                temp /= pr;
                ++exp;
            }
            if (exp & 1)          /* odd exponent contributes pr */
                kernel *= (seq_t)pr;
        }

        seq_t sqrtk = floor_sqrt((seq_t)k);
        if (kernel > sqrtk) {      /* satisfies condition */
            if (vals_len == vals_cap) {
                size_t new_cap = vals_cap ? vals_cap * 2 : 64;
                seq_t *new_vals = mem_resize(seq_t, vals, new_cap);
                if (!new_vals) {
                    /* allocation failed */
                    vals = NULL;
                    vals_cap = 0;
                    vals_len = 0;
                    return 0;
                }
                vals = new_vals;
                vals_cap = new_cap;
            }
            vals[vals_len++] = (seq_t)k;
        }
    }

    return vals[n - 1];
}
