/*
 * A107203  Primes of the form 9x^2 + 10y^2.
 * https://oeis.org/A107203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A107203 – primes of the form 9x^2 + 10y^2.
 *  a(n) is the nth prime p for which ∃ integers x,y with p = 9x^2 + 10y^2.
 *  This implementation searches primes in increasing order, testing each
 *  candidate for the representation by a direct scan over possible y.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

static size_t     primes_cap   = 0;
static uint64_t * primes_buf   = NULL;
static size_t     primes_cnt   = 0;
static uint64_t   last_checked = 2;

/* test whether a prime p can be written as 9x^2 + 10y^2 */
static bool is_of_form(uint64_t p)
{
    for (uint64_t y = 0; 10 * y * y <= p; ++y) {
        uint64_t y2 = 10 * y * y;
        if (p < y2) break;
        uint64_t rem = p - y2;
        if (rem % 9 != 0) continue;
        uint64_t x2 = rem / 9;
        uint64_t s  = int_sqrt(x2);
        if (s * s == x2) return true;
    }
    return false;
}

/* ensure primes_buf has room for at least one more prime */
static void ensure_capacity(void)
{
    if (primes_buf == NULL) {
        primes_cap = 64;
        primes_buf = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (primes_buf == NULL) abort();
    } else if (primes_cnt >= primes_cap) {
        primes_cap *= 2;
        primes_buf = (uint64_t*)realloc(primes_buf,
                                       primes_cap * sizeof(uint64_t));
        if (primes_buf == NULL) abort();
    }
}

/* produce the next prime representable by 9x^2 + 10y^2 */
static void generate_next(void)
{
    for (;;) {
        if (is_prime(last_checked) && is_of_form(last_checked)) {
            ensure_capacity();
            primes_buf[primes_cnt++] = last_checked;
            last_checked++;
            return;
        }
        last_checked++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (primes_cnt < n) generate_next();
    return (seq_t)primes_buf[n - 1];
}
