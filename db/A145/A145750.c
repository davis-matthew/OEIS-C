/*
 * A145750  Primes which become emirps when rotated by 180 degrees on a
 *          digital clock display.
 * https://oeis.org/A145750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A145750: primes p such that rotating p by 180°
   (digits 0,1,6,8,9) yields a prime r that is an emirp
   (prime whose reverse is a different prime). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

static const int mp[10] = { 0, 1, -1, -1, -1, -1, 9, -1, 8, 6 };

/* 180° rotation of a decimal number using only allowable digits.
   Returns 0 if the number contains an invalid digit. */
static inline seq_t rotate_prime(seq_t n)
{
    seq_t res = 0;
    while (n > 0)
    {
        int d = (int)(n % 10);
        int m = mp[d];
        if (m < 0)
            return 0;          /* contains forbidden digit */
        res = res * 10 + m;
        n /= 10;
    }
    return res;
}

/* Reverse decimal digits of n. */
static inline seq_t reverse_digits(seq_t n)
{
    seq_t rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t cap   = 0;
    static size_t size  = 0;
    static uint64_t last_prime = 1;   /* prime > last used */

    if (n == 0)
        return 0;          /* sequence starts at index 1 */

    while (size < (size_t)n)
    {
        last_prime = next_prime(last_prime);
        seq_t p = (seq_t)last_prime;

        // rotate
        seq_t rt = rotate_prime(p);
        if (rt == 0)
            continue;       /* contains invalid digit */

        // rotated must be prime
        if (!is_prime_big(rt))
            continue;

        // rotated must be an emirp
        seq_t rev = reverse_digits(rt);
        if (rev == rt)          /* palindrome, not an emirp */
            continue;
        if (!is_prime_big(rev))
            continue;

        /* store term */
        if (size >= cap)
        {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = seq_realloc(terms, cap, new_cap);
            terms = tmp;
            cap   = new_cap;
        }
        terms[size++] = p;
    }

    return terms[n - 1];
}
