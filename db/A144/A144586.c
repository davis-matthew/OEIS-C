/*
 * A144586  a(n) = odd and nonprime sum (with multiplicity) of prime factors
 *          of composite(k) = A002808(k).
 * https://oeis.org/A144586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9564 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A144586: a(n) = sum of prime factors (with multiplicity) of the n‑th
 * composite number, when that sum is an odd composite number. The sequence
 * is built by iterating composite numbers and caching results. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Cache of the sequence values */
    static seq_t *cached   = NULL;
    static size_t cap      = 0, len = 0;
    static uint64_t next_comp = 4;
    static bool initialized = false;

    if (n == 0)
        return (seq_t)0;                     /* sequence is undefined at 0 */

    if (!initialized) {
        cap = 16;
        cached = seq_alloc(cap);             /* allocates an array of seq_t */
        if (!cached)
            return (seq_t)0;
        len = 0;
        next_comp = 4;
        initialized = true;
    }

    while (len < n) {
        /* Find the next composite number */
        while (is_prime(next_comp))
            ++next_comp;
        uint64_t N = next_comp;

        /* Sum of prime factors (with multiplicity) */
        seq_t sum = (seq_t)0;
        uint64_t tmp = N;
        while (tmp > 1) {
            uint64_t p = smallest_prime_factor(tmp);
            if (p == 0)                        /* defensive, should not happen */
                break;
            sum += (seq_t)p;
            tmp /= p;
        }

        /* Check for odd composite sum */
        if ((sum % (seq_t)2) == (seq_t)1 && sum > (seq_t)1 && !is_prime_big(sum)) {
            /* Store the term, growing the cache if necessary */
            if (len + 1 > cap) {
                size_t newcap = cap * 2;
                seq_t *tmp_arr = seq_realloc(cached, cap, newcap);
                if (!tmp_arr)
                    return (seq_t)0;
                cached = tmp_arr;
                cap = newcap;
            }
            cached[len++] = sum;
        }

        /* Advance to the next integer for the next iteration */
        ++next_comp;
    }

    return cached[n - 1];
}
