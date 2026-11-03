/*
 * A038530  Concatenate n-th prime and n-th composite.
 * https://oeis.org/A038530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(prime(n), composite(n)), with primes and
 *            composites counted in ascending order starting at 1
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Cache for primes and composites */
static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static idx_t *comp_cache  = NULL;
static size_t comp_cnt  = 0, comp_cap = 0;
static idx_t next_num = 2;  /* next number to test for primality/compositeness */

/* Ensure that the caches hold at least 'need' entries each. */
static void ensure_cache(idx_t need)
{
    if (prime_cnt >= need && comp_cnt >= need)
        return;

    while (prime_cnt < need || comp_cnt < need) {
        /* Skip numbers <= 1 (none are composite). */
        if (next_num <= 1) {
            ++next_num;
            continue;
        }

        if (is_prime(next_num)) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_cache = (idx_t *)realloc(prime_cache,
                                               prime_cap * sizeof(idx_t));
            }
            prime_cache[prime_cnt++] = next_num;
        } else {
            if (comp_cnt == comp_cap) {
                comp_cap = comp_cap ? comp_cap * 2 : 16;
                comp_cache = (idx_t *)realloc(comp_cache,
                                              comp_cap * sizeof(idx_t));
            }
            comp_cache[comp_cnt++] = next_num;
        }
        ++next_num;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_cache(n);

    /* prime_cache[n-1] and comp_cache[n-1] are of type idx_t,
       which is the expected type for concat_parts arguments. */
    return concat_parts(prime_cache[n - 1], comp_cache[n - 1]);
}
