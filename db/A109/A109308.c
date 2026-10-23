/*
 * A109308  Lesser emirps (primes whose digit reversal is a larger prime).
 * https://oeis.org/A109308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth prime p such that reverse(p) > p and reverse(p)
 *            is also prime
 * Range:     u8 n <= 11, u16 n <= 666, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"   /* bool is_prime_uint64(uint64_t) */

static seq_t *lesser    = NULL;   /* array of found terms */
static idx_t  less_cap  = 0;      /* allocated capacity of the array */
static idx_t  less_cnt  = 0;      /* number of terms stored */
static uint64_t next_candidate = 3;   /* next odd number to test */

/* reverse an unsigned 64‑bit integer */
static uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* append a new term to the array, growing it if necessary */
static void add_term(seq_t val)
{
    if (less_cnt == less_cap) {
        idx_t new_cap = (less_cap == 0) ? 128 : less_cap * 2;
        seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (lesser) {
            for (idx_t i = 0; i < less_cnt; ++i)
                new_ptr[i] = lesser[i];
            free(lesser);
        }
        lesser = new_ptr;
        less_cap = new_cap;
    }
    lesser[less_cnt++] = val;
}

/* main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but guard */

    while (less_cnt < n) {
        /* test next_candidate */
        if (is_prime_uint64(next_candidate)) {
            uint64_t rev = rev_uint64(next_candidate);
            if (rev > next_candidate && is_prime_uint64(rev)) {
                add_term((seq_t)next_candidate);
            }
        }
        next_candidate += 2;  /* only odd numbers are needed after 3 */
    }

    return lesser[n - 1];
}
