/*
 * A162576  Primes of the form x^2+2^x+y^2+2^y, with x and y nonnegative.
 * https://oeis.org/A162576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   prime numbers of the form x^2 + 2^x + y^2 + 2^y with x and y
 *            positive integers (x ≥ 1, y ≥ 1), listed in increasing order.
 *            a(n) is the nth such prime (a(1)=11)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation incrementally generates all unordered pairs
   (x,y) with x ≤ y.  For each new maximum coordinate y = L,
   it evaluates all pairs (x, L) with 1 ≤ x ≤ L, tests
   primality, and inserts the resulting prime into a static
   sorted array.  The array expands only as far as needed to
   answer the largest n requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

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

static int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Insert x into a sorted array arr[0..cnt-1] if not already present.
   arr may be reallocated by ensure_capacity, and *arrp updated. */
static void
insert_sorted(seq_t **arrp, size_t *cnt, size_t *cap, seq_t x)
{
    seq_t *arr = *arrp;
    size_t lo = 0, hi = *cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int cmp = compare_seq_t(&arr[mid], &x);
        if (cmp < 0)
            lo = mid + 1;
        else if (cmp > 0)
            hi = mid;
        else
            return;                 /* duplicate */
    }
    ensure_capacity(*cnt + 1, cap, OEISC_VPP(arrp), sizeof(seq_t));
    arr = *arrp;                      /* arr may have moved after realloc */
    mem_move(arr + lo + 1, arr + lo, (*cnt - lo) * sizeof(seq_t));
    arr[lo] = x;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* static state ----------------------------------------------------------
     *   primes:     sorted list of all primes found so far
     *   primes_cnt: number of primes stored
     *   primes_cap: capacity of primes array
     *   limit:      current maximum value of x and y seen
     *   pow2:       array of powers of two, pow2[i] = 2^i
     *   pow2_cap:   capacity of pow2 array
     *   pow2_cnt:   number of entries in pow2 (equals limit + 1)
     */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *pow2 = NULL;
    static size_t pow2_cap = 0, pow2_cnt = 0;
    static idx_t limit = 0;

    /* initialize pow2[0] */
    if (pow2_cnt == 0) {
        ensure_capacity(1, &pow2_cap, OEISC_VPP(&pow2), sizeof(seq_t));
        pow2[0] = (seq_t)1;
        pow2_cnt = 1;
    }

    /* generate new candidates until we have at least n primes */
    while (primes_cnt < (size_t)n) {
        limit++;                         /* new y = limit */

        /* extend pow2 array if necessary */
        if ((size_t)limit >= pow2_cap)
            ensure_capacity(limit + 1, &pow2_cap, OEISC_VPP(&pow2), sizeof(seq_t));

        /* compute 2^limit from previous value */
        pow2[limit] = pow2[limit - 1] * (seq_t)2;
        pow2_cnt = limit + 1;

        seq_t pow_limit = pow2[limit];
        seq_t limit_sq = (seq_t)limit * (seq_t)limit;

        /* consider all pairs (x, limit) with 1 ≤ x ≤ limit */
        for (idx_t x = 1; x <= limit; ++x) {
            seq_t pow_x = pow2[x];
            seq_t val = (seq_t)x * (seq_t)x + pow_x + limit_sq + pow_limit;
            if (is_prime_big(val))
                insert_sorted(&primes, &primes_cnt, &primes_cap, val);
        }
    }

    return primes[n - 1];
}
