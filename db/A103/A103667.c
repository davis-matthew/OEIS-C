/*
 * A103667  Primes p such that the largest prime divisor of p-1 is greater
 *          than the largest prime divisor of p+1.
 * https://oeis.org/A103667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3244, u32 n <= 39328, u64 n <= 39328,
 *            u128 n <= 39328, big any n
 * Validated: standard, pass, 71 terms through n = 9319 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime p such that
   largest_prime_factor(p-1) > largest_prime_factor(p+1) */

/* We generate primes on demand, test the condition, and memoize
   the sequence values. All state is kept in static variables so
   calls may come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

static seq_t *values      = NULL;   /* array of computed sequence terms */
static idx_t  values_count = 0;     /* number of stored terms   */
static idx_t  values_cap   = 0;     /* allocated capacity */

static uint64_t curr = 2;           /* next integer to test for primality */

/* Ensure the values array has room for at least additional slots. */
static void ensure_values_capacity(idx_t additional)
{
    idx_t needed = values_count + additional;
    if (needed > values_cap) {
        idx_t newcap = values_cap ? values_cap * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = mem_resize(seq_t, values, newcap);
        values = tmp;
        values_cap = newcap;
    }
}

/* Append a new value to the sequence array. */
static void add_value(seq_t val)
{
    ensure_values_capacity(1);
    values[values_count++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined for n=0 */

    /* Generate terms until we have at least n of them. */
    while (values_count < n) {
        if (is_prime_u64(curr)) {
            seq_t p   = (seq_t)curr;
            seq_t lpf1 = greatest_prime_factor(p - (seq_t)1);
            seq_t lpf2 = greatest_prime_factor(p + (seq_t)1);
            if (lpf1 > lpf2) {
                add_value(p);
            }
        }
        /* Advance to the next integer. */
        ++curr;
        /* Avoid overflow of curr for 64-bit logic; if it overflows,
           the loop will terminate because no larger primes fit. */
        if (curr == 0) break;
    }
    return values[n - 1];
}
