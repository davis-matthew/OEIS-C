/*
 * A071224  LCM of n and n-th composite number.
 * https://oeis.org/A071224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, C_n) = n*C_n/gcd(n, C_n) where C_n is the n-th
 *            composite number (offset 1)
 * Range:     u8 n <= 12, u16 n <= 226, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

static seq_t *comp_cache = NULL;       /* array of previously found composites */
static size_t comp_cap = 0;            /* allocated capacity of comp_cache   */
static size_t comp_count = 0;          /* number of composites stored       */
static idx_t last_checked = 3;         /* last integer examined for compositeness */

/* ensure the cache can hold 'needed' elements */
static void comp_grow(size_t needed)
{
    if (comp_cap == 0) {
        comp_cap = 64;
    }
    while (comp_cap < needed) {
        comp_cap <<= 1;
    }
    comp_cache = (seq_t *)realloc(comp_cache, comp_cap * sizeof(seq_t));
    /* in a library build realloc failure would be handled elsewhere */
}

/* return the n-th composite number (1‑based) */
static seq_t nth_composite(idx_t n)
{
    while (comp_count < n) {
        last_checked++;
        /* numbers >1 that are not prime are composite */
        if (last_checked > 1 && !is_prime(last_checked)) {
            if (comp_cap == comp_count) {
                comp_grow(comp_count + 1);
            }
            comp_cache[comp_count++] = (seq_t)last_checked;
        }
    }
    return comp_cache[n - 1];
}

/* main sequence function requested by the problem */
seq_t a(idx_t n)
{
    seq_t cn = nth_composite(n);            /* composite number */
    seq_t nn = (seq_t)n;                    /* term index as seq_t */
    seq_t g  = gcd(nn, cn);
    return (nn / g) * cn;                   /* lcm computation */
}
