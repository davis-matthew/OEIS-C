/*
 * A106873  Primes of the form 2x^2-xy+4y^2, with x and y nonnegative.
 * https://oeis.org/A106873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime of the form 2x^2 - xy + 4y^2 with x,y >= 0
 * Range:     u8 n <= 12, u16 n <= 1227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt, is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/integer_sqrt.h"
#include "../utils/push_seq.h"

/* Test whether a prime p can be represented as 2x^2 - xy + 4y^2
   with non‑negative integers x and y. */
static bool is_representable(uint64_t p)
{
    uint64_t y = 0;
    /* The condition 31*y^2 <= 8*p guarantees D >= 0. */
    while (1) {
        __int128 lhs = (__int128)31 * (__int128)y * (__int128)y;
        __int128 rhs = (__int128)8  * (__int128)p;
        if (lhs > rhs)
            break;

        __int128 D = rhs - lhs;        /* D = 8p - 31y^2 */
        idx_t D64 = (idx_t)D;          /* fits in idx_t for all n up to about 10^7 */
        idx_t sqrtD = integer_sqrt(D64);
        if ((__int128)sqrtD * sqrtD == D) {
            /* Corresponding x values: (y ± sqrtD)/4  */
            if ((y + sqrtD) % 4 == 0)
                return true;
            if (y >= sqrtD && (y - sqrtD) % 4 == 0)
                return true;
        }
        y++;
    }
    return false;
}

/* Dynamic array of found primes of the form.  Static state makes
   the function order‑independent. */
static seq_t *prime_array = NULL;
static size_t prime_cap   = 0;
static idx_t  prime_cnt   = 0;
static uint64_t current   = 2;      /* next integer to test */

/* Generate enough terms to cover index n.  Calls to a() will
   automatically extend the table. */
static void generate_upto(idx_t n)
{
    while (prime_cnt < n) {
        if (is_prime(current)) {
            if (is_representable(current)) {
                push_seq(&prime_array, &prime_cap, &prime_cnt, (seq_t)current);
            }
        }
        current++;
    }
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n >= 1 this function is valid. */
    if (n == 0)
        return 0;           /* undefined, but harmless */
    generate_upto(n);
    return prime_array[n - 1];
}
