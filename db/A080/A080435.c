/*
 * A080435  a(1) = 1; a(n) = least k > a(n-1) such that each prime of form
 *          a(i)+a(j) occurs for unique i <= j.
 * https://oeis.org/A080435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080435
   a(1) = 1
   a(n) = least k > a(n‑1) such that all prime sums a(i)+a(j) for
   1 ≤ i ≤ j ≤ n are distinct.  The function builds the sequence
   incrementally and memoises terms for arbitrary calling order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Helper: compare seq_t values for equality.
   The operators == are defined for all allowed seq_t types. */
static inline bool seq_t_eq(seq_t x, seq_t y) { return x == y; }

/* Forward declaration of auxiliary array structure */
typedef struct { seq_t *data; size_t len; size_t cap; } arr_t;

/* Ensure an array has at least `need` elements allocated.  The array
   objects are constructed/destructed correctly in C++ using new/delete
   and with malloc/free in C. */
static void array_ensure(arr_t *a, size_t need)
{
    if (need <= a->cap) return;
    size_t new_cap = a->cap ? a->cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_data = new seq_t[new_cap];
#else
    seq_t *new_data = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    for (size_t i = 0; i < a->len; ++i) new_data[i] = a->data[i];
#ifdef __cplusplus
    delete[] a->data;
#else
    free(a->data);
#endif
    a->data = new_data;
    a->cap = new_cap;
}

/* Static state shared between calls */
static arr_t seq  = {NULL, 0, 0};   /* sequence values a(1)..a(n)           */
static arr_t used = {NULL, 0, 0};   /* primes that have already appeared   */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety – unused */

    /* Ensure we have at least `n` terms pre‑computed. */
    while (seq.len < (size_t)n) {
        /* First term */
        if (seq.len == 0) {
            array_ensure(&seq, 1);
            seq.data[0] = (seq_t)1;   /* a(1) = 1 */
            seq.len = 1;
            continue;
        }

        /* Candidate `k` starts at previous term + 1. */
        seq_t prev = seq.data[seq.len - 1];
        seq_t k = prev + (seq_t)1;

        /* Search for the first acceptable candidate `k`. */
        for (;;) {
            bool ok = true;
            size_t new_needed = seq.len + 1;
            seq_t *new_primes = NULL;
#ifdef __cplusplus
            new_primes = new seq_t[new_needed];
#else
            new_primes = (seq_t *)malloc(new_needed * sizeof(seq_t));
#endif
            size_t new_len = 0;

            /* Check sums `a[i] + k` for i < n. */
            for (size_t i = 0; i < seq.len && ok; ++i) {
                seq_t sum = seq.data[i] + k;
                if (is_prime_big(sum)) {
                    /* Duplicate with previously used primes? */
                    for (size_t j = 0; j < used.len; ++j)
                        if (seq_t_eq(used.data[j], sum)) { ok = false; break; }
                    if (!ok) break;
                    /* Duplicate among new sums for this `k`? */
                    for (size_t j = 0; j < new_len; ++j)
                        if (seq_t_eq(new_primes[j], sum)) { ok = false; break; }
                    if (!ok) break;
                    new_primes[new_len++] = sum;
                }
            }

            /* Check the self–sum `k + k`. */
            if (ok) {
                seq_t selfsum = k + k;
                if (is_prime_big(selfsum)) {
                    /* Existing prime? */
                    for (size_t j = 0; j < used.len; ++j)
                        if (seq_t_eq(used.data[j], selfsum)) { ok = false; break; }
                    if (!ok) {
                        /* free new_primes before retrying */
                        goto free_new_primes_and_inc;
                    }
                    /* Duplicate among new sums for this `k`? */
                    for (size_t j = 0; j < new_len; ++j)
                        if (seq_t_eq(new_primes[j], selfsum)) { ok = false; break; }
                    if (!ok) {
                        goto free_new_primes_and_inc;
                    }
                    new_primes[new_len++] = selfsum;
                }
            }

            /* If candidate is acceptable, break the search loop. */
            if (ok) {
                /* Append `k` to the sequence. */
                array_ensure(&seq, seq.len + 1);
                seq.data[seq.len++] = k;

                /* Store all new prime sums for future uniqueness checks. */
                for (size_t j = 0; j < new_len; ++j) {
                    array_ensure(&used, used.len + 1);
                    used.data[used.len++] = new_primes[j];
                }

#ifdef __cplusplus
                delete[] new_primes;
#else
                free(new_primes);
#endif
                break;
            }

free_new_primes_and_inc:
#ifdef __cplusplus
            delete[] new_primes;
#else
            free(new_primes);
#endif
            ++k;                    /* try next candidate */
        }   /* end for(;;) */
    }   /* end while(pre-computation) */

    return seq.data[n - 1];
}
