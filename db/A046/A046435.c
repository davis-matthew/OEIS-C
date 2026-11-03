/*
 * A046435  Numbers such that 5 steps are needed to reach a prime under "Sum
 *          of digits raised to its digits' powers" procedure.
 * https://oeis.org/A046435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer for which the map * f(x) = Σ digit^digit
 *            (with 0^0 = 1) reaches a prime after * exactly five iterations
 *            (f^5(x) is prime and * f^k(x) is composite for k = 1…4)
 * Range:     u8 none, u16 none, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
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

/* Pre‑computed powers d^d for 0 ≤ d ≤ 9  (treating 0^0 = 1). */
static seq_t pow_table[10];
static bool pow_table_init = false;

static inline void init_pow_table(void)
{
    if (pow_table_init)
        return;
    pow_table[0] = (seq_t)1;                      /* 0^0 = 1 */
    for (int d = 1; d <= 9; ++d)
        pow_table[d] = ipow_idx((seq_t)d, d);      /* d^d */
    pow_table_init = true;
}

/* Sum of digits raised to their own powers, with 0^0 = 1. */
static inline seq_t digit_power_sum(seq_t x)
{
    init_pow_table();
    seq_t sum  = (seq_t)0;
    seq_t ten  = (seq_t)10;
    seq_t zero = (seq_t)0;

    while (x != zero) {
        seq_t d_seq = x % ten;
        unsigned d = (unsigned)d_seq;            /* 0 ≤ d < 10 */
        sum += pow_table[d];
        x /= ten;
    }
    return sum;
}

/* True iff n requires exactly five steps to reach a prime. */
static inline bool reaches_prime_in_five_steps(seq_t n)
{
    seq_t val = n;
    for (int step = 1; step <= 5; ++step) {
        val = digit_power_sum(val);
        bool prime = is_prime_big(val);
        if (step < 5 && prime)          return false;  /* prime too early */
        if (step == 5 && !prime)        return false;  /* final not prime */
    }
    return true;
}

/* Cache of discovered terms. */
static seq_t *cache         = NULL;
static size_t cache_cap     = 0;
static size_t cache_count   = 0;
static idx_t   next_to_test = 1;      /* next integer to examine */

/* Ensure cache has room for at least needed elements. */
static inline void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined below offset 1 */
        return (seq_t)0;

    while (cache_count < n) {
        if (reaches_prime_in_five_steps((seq_t)next_to_test)) {
            ensure_cache(cache_count + 1);
            cache[cache_count++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
