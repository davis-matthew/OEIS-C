/*
 * A062016  Numbers with all odd digits which can be factored into two numbers
 *          > 1 with odd digits.
 * https://oeis.org/A062016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9993 (2026-11-03)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062016 – Numbers with all odd digits that can be factored into two
   factors >1, each having only odd digits.  The function returns the
   n‑th term (1‑based).  Terms are generated on demand and cached in a
   static dynamic array that grows with the helper grow_seq_array. */

/* Include helpers from the utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Check if every decimal digit of x is odd [1,3,5,7,9]. */
static inline bool all_odd_digits(unsigned long long x)
{
    if (x == 0) return false;           /* 0 has an even digit 0 */
    while (x) {
        unsigned d = x % 10;
        if ((d & 1) == 0) return false; /* even digit found */
        x /= 10;
    }
    return true;
}

/* Test whether n has a non‑trivial factorisation into two odd‑digit numbers. */
static inline bool has_odd_factorization(unsigned long long n)
{
    if (n <= 1) return false;
    unsigned long long limit = isqrt_u64(n);
    for (unsigned long long d = 2; d <= limit; ++d) {
        if (n % d == 0) {
            unsigned long long q = n / d;
            if (all_odd_digits(d) && all_odd_digits(q))
                return true;
        }
    }
    return false;
}

/* --------------------------------------------------------------- */
/* Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t   *terms   = NULL;   /* cached terms */
    static size_t  count   = 0;       /* number of stored terms */
    static size_t  capacity = 0;      /* allocated space for terms */
    static unsigned long long cur = 1;/* last tested integer */

    if (n == 0) return 0;             /* undefined, but harmless */

    /* Generate terms until we have at least n of them */
    while (count < n) {
        ++cur;
        if (!all_odd_digits(cur)) continue;
        if (!has_odd_factorization(cur)) continue;

        /* Grow the array if full */
        if (count == capacity) {
            size_t newcap = capacity ? capacity << 1 : 16;
            grow_seq_array(&terms, &capacity, newcap);
        }
        terms[count++] = (seq_t)cur;
    }

    return terms[n - 1];
}
