/*
 * A074039  If (n, n+2) is the k-th twin prime pair then k else 0.
 * https://oeis.org/A074039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k if (n, n+2) is the k‑th twin prime pair, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Cache of the first element of each twin prime pair */
static idx_t *twin_first = NULL; /* array of first elements of twin primes */
static size_t twin_count = 0;    /* number of twin primes found so far */
static size_t twin_cap   = 0;    /* allocated capacity of twin_first */
static idx_t  max_checked = 0;   /* largest n that has been checked */

/* Extend twin_first with all twin primes up to n (inclusive) */
static void ensure_twin_up_to(idx_t n)
{
    while (max_checked < n) {
        ++max_checked;
        /* test whether (max_checked, max_checked+2) is a twin prime pair */
        if (is_prime(max_checked) && is_prime(max_checked + (idx_t)2)) {
            if (twin_count == twin_cap) {
                size_t new_cap = (twin_cap == 0) ? 4 : twin_cap * 2;
                idx_t *tmp = (idx_t *)realloc(twin_first, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                twin_first = tmp;
                twin_cap   = new_cap;
            }
            twin_first[twin_count++] = max_checked;
        }
    }
}

/* Binary search for n in twin_first.
   Returns 0‑based index if found, otherwise -1. */
static long long twin_index(idx_t n)
{
    size_t lo = 0, hi = twin_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (twin_first[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < twin_count && twin_first[lo] == n) ? (long long)lo : -1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_twin_up_to(n);             /* extend twin list to n */
    long long idx = twin_index(n);    /* 0‑based index of n if twin prime */
    return (idx >= 0) ? (seq_t)(idx + 1) : (seq_t)0;
}
