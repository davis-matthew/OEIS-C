/*
 * A066039  Largest prime less than or equal to the sum of first n primes
 *          (A007504).
 * https://oeis.org/A066039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   largest prime <= sum of first n primes, OEIS A066039
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 929 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
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

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n<1, return 0 to avoid crash */
        return 0;

    /* Static state */
    static seq_t *sum_arr  = NULL;   /* cumulative sums of primes   */
    static uint64_t *prime_arr = NULL; /* list of primes              */
    static size_t sum_cap = 0;
    static size_t prime_cap = 0;
    static size_t sum_cnt = 0;   /* number of sums computed (=number of primes) */
    static size_t prime_cnt = 0;

    /* Fill tables up to requested n */
    while (sum_cnt < (size_t)n)
    {
        /* Find next prime */
        uint64_t cand = 2;
        if (prime_cnt > 0)
            cand = prime_arr[prime_cnt - 1] + 1;

        while (!is_prime_seq(cand))
            ++cand;

        /* Store prime */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            ensure_capacity(new_cap, &prime_cap, OEISC_VPP(&prime_arr),
                            sizeof(uint64_t));
        }
        prime_arr[prime_cnt++] = cand;

        /* Store cumulative sum */
        seq_t new_sum = (sum_cnt == 0) ? (seq_t)cand
                                       : sum_arr[sum_cnt - 1] + (seq_t)cand;
        if (sum_cnt == sum_cap) {
            size_t new_cap = sum_cap ? sum_cap * 2 : 8;
            ensure_capacity(new_cap, &sum_cap, OEISC_VPP(&sum_arr), sizeof(seq_t));
        }
        sum_arr[sum_cnt++] = new_sum;
    }

    /* Find largest prime <= the sum */
    seq_t target = sum_arr[n - 1];
    seq_t result = target;

    while (!is_prime_big(result))
        result--;

    return result;
}
