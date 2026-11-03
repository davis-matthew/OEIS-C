/*
 * A068951  Scan the primes, record digit-sum if it is itself prime.
 * https://oeis.org/A068951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digit_sum(p_n) where p_n is the n‑th prime and
 *            digit_sum(p_n) is prime. (n starts at 1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-03)
 * Helpers:   is_prime_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_i32.h"
#include "../utils/mem.h"

/* sum of decimal digits of an unsigned integer */
static inline int digit_sum_uint32(uint32_t x)
{
    int s = 0;
    while (x) { s += x % 10; x /= 10; }
    return s;
}

/* grow a dynamically allocated array of seq_t */
static void grow_seq_t_arr(seq_t **arr, size_t *cap)
{
    if (*cap == 0)        *cap = 64;
    else                  *cap <<= 1;
    *arr = mem_resize(seq_t, *arr, (*cap * sizeof(seq_t)) / sizeof(seq_t));
}

/* ---- small helpers --------------------------------------------------- */

/* primality test for primes that appear in the sequence.
   The p_n we need for n ≤ 10000 are below 200000, fit in uint32_t. */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2)            return false;
    if (n == 2)           return true;
    if ((n & 1) == 0)     return false;
    for (uint32_t p = 3; (uint64_t)p * p <= n; p += 2)
        if (n % p == 0)   return false;
    return true;
}

/* ---- sequence implementation ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */

    static seq_t *vals      = NULL;
    static size_t vals_len  = 0;
    static size_t vals_cap  = 0;
    static uint32_t next_candidate = 2;   /* next prime to try */

    while (vals_len < n) {
        /* find next prime starting from next_candidate */
        uint32_t p;
        for (p = next_candidate; ; ++p) {
            if (is_prime_uint32(p)) break;
        }
        next_candidate = p + 1;           /* cache for next call */

        /* digit sum */
        int ds = digit_sum_uint32(p);
        if (is_prime_i32(ds)) {
            if (vals_len >= vals_cap) grow_seq_t_arr(&vals, &vals_cap);
            vals[vals_len++] = (seq_t)ds;
        }
    }
    return vals[n - 1];
}
