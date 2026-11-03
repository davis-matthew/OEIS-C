/*
 * A180528  Primes that become a different prime under the mapping 1 <=> 5.
 * https://oeis.org/A180528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 719, u32 n <= 2200, u64 n <= 2200,
 *            u128 n <= 2200, big any n
 * Validated: standard, pass, 62 terms through n = 1836 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180528: primes that become a different prime when digits 1 and 5 are swapped.
   Search for primes p such that p ≠ map(p), both p and map(p) are prime.
   Sequence is indexed starting at 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/is_prime.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* map_value swaps 1↔5 in the decimal representation of x. */
static seq_t map_value(seq_t x)
{
    if (x < 10) {
        /* single digit – just swap if necessary */
        if (x == 1) return 5;
        if (x == 5) return 1;
        return x;
    }

    /* count digits */
    idx_t digits = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        ++digits;
        tmp /= 10;
    }

    /* prepare 10^(digits-1) */
    seq_t power = pow10(digits - 1);
    seq_t result = 0;

    /* extract digits from most significant to least */
    while (x > 0) {
        seq_t digit = x / power;          /* most significant digit */
        seq_t rem   = x % power;          /* remainder */
        if (digit == 1) digit = 5;
        else if (digit == 5) digit = 1;
        result = result * 10 + digit;
        x      = rem;
        power  /= 10;                     /* move to next lower place */
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n < 1 */

    static seq_t *results = NULL;   /* array of qualifying primes */
    static size_t count   = 0;      /* number of stored primes   */
    static size_t cap     = 0;      /* current allocation size  */
    static uint64_t cur    = 1;     /* last integer checked      */

    while (count < n) {
        ++cur;
        if (!is_prime(cur))                 /* skip non‑primes  */
            continue;

        seq_t mapped = map_value((seq_t)cur);
        if (mapped == (seq_t)cur)           /* mapping unchanged */
            continue;

        if (!is_prime_big(mapped))          /* mapped number not prime */
            continue;

        /* store the qualifying prime */
        ensure_capacity(count + 1, &cap, OEISC_VPP(&results), sizeof(seq_t));
        results[count++] = (seq_t)cur;
    }

    return results[n - 1];   /* zero‑based index into the array */
}
