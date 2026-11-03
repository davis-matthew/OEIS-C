/*
 * A234739  Largest prime divisor of all composite numbers of the form k^2+1
 *          between two consecutive primes of the same form.
 * https://oeis.org/A234739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor among all composite k^2+1 numbers
 *            lying strictly between consecutive primes of the form m^2+1
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses the utility routines supplied by the build
    environment:  is_prime_big, largest_prime_divisor, and
    _floor_isqrt_u64.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isqrt.h"
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

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

/* Dynamic array growth helper */
#ifdef __cplusplus
template <class T>
static void
ensure_capacity(idx_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if ((size_t)needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    *ptr = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    *cap = new_cap;
}
#else
static void
ensure_capacity(idx_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if ((size_t)needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}
#endif

/* Main sequence routine */
seq_t a(idx_t n)
{
    /*  Storage for primes of the form m^2+1 and the sequence values. */
    static seq_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static uint64_t next_m = 2;       /* start at m = 2 (gives 5) */

    static seq_t *a_values = NULL;
    static size_t a_cnt = 0, a_cap = 0;

    /* Ensure we have at least primes[n] and primes[n+1] */
    idx_t needed_prime_index = n + 1;     /* 0‑based index of last needed prime */
    while (prime_cnt <= (size_t)needed_prime_index) {
        uint64_t cand = (uint64_t)next_m * next_m + 1;
        if (is_prime_big((seq_t)cand)) {
            ensure_capacity(prime_cnt + 1, &prime_cap, OEISC_VPP(&prime_list),
                            sizeof(seq_t));
            prime_list[prime_cnt++] = (seq_t)cand;
        }
        ++next_m;                         /* try next m */
    }

    /* Compute a values up to n if required */
    while (a_cnt < (size_t)n) {
        idx_t idx = a_cnt;                     /* will compute a(idx+1)   */
        uint64_t lower = (uint64_t)prime_list[idx];
        uint64_t upper = (uint64_t)prime_list[idx + 1];

        uint64_t max_lpd = 0;
        uint64_t start_m = isqrt(lower - 1) + 1;
        for (uint64_t m = start_m; ; ++m) {
            uint64_t num = m * m + 1;
            if (num >= upper) break;
            if (!is_prime_big((seq_t)num)) {
                uint64_t lpd = largest_prime_divisor(num);
                if (lpd > max_lpd) max_lpd = lpd;
            }
        }

        ensure_capacity(a_cnt + 1, &a_cap, OEISC_VPP(&a_values), sizeof(seq_t));
        a_values[a_cnt++] = (seq_t)max_lpd;
    }

    return a_values[n - 1];
}
