/*
 * A112381  Zero-free semiprimes.
 * https://oeis.org/A112381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zero‑free semiprimes: the product of exactly two primes that contains no digit 0.
   The nth term a(n) is the nth such integer (a(1) = 4). */
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Return true if x contains the digit 0. */
static bool has_zero_digit(seq_t x)
{
    while (x > 0)
    {
        if ((x % (seq_t)10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Memoised list of zero‑free semiprimes. */
    static seq_t *table = NULL;
    static idx_t sz   = 0;   /* number of terms stored */
    static idx_t cap  = 0;   /* capacity of the array */
    static idx_t cand = 2;   /* next integer to test */

    while (sz < n)
    {
        uint64_t u = (uint64_t)cand;
        if (is_semiprime_uint64(u) && !has_zero_digit(cand))
        {
            push_seq(&table, &cap, &sz, (seq_t)cand);
        }
        ++cand;
    }

    return table[n - 1];
}
