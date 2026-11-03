/*
 * A154764  Primes with exactly one odd decimal digit.
 * https://oeis.org/A154764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime that has exactly one odd decimal digit
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

// start checking from 3 (all odd numbers)
static bool has_one_odd_digit(idx_t x)
{
    int odd = 0;
    while (x > 0)
    {
        int d = x % 10;
        if (d & 1)
        {
            odd++;
            if (odd > 1)
                return false;
        }
        x /= 10;
    }
    return odd == 1;
}

static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static idx_t next_num = 3;

static void ensure_capacity(size_t needed)
{
    if (primes_cap >= needed)
        return;
    size_t newcap = primes_cap ? primes_cap * 2 : 64;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = seq_realloc(primes, primes_cap, newcap);
    primes   = tmp;
    primes_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    // undefined for n <= 0

    if (!primes)
    {
        primes = seq_alloc_new_array(64);
        primes_cap = 64;
    }

    while (primes_len < (size_t)n)
    {
        if (is_prime(next_num) && has_one_odd_digit(next_num))
        {
            ensure_capacity(primes_len + 1);
            primes[primes_len++] = (seq_t)next_num;
        }
        next_num += 2;      // skip even numbers
    }
    return primes[(size_t)n - 1];
}
