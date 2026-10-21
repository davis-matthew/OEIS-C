/*
 * A066819  Sum of the first n Sophie Germain primes.
 * https://oeis.org/A066819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 8476, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066819 – Sum of the first n Sophie Germain primes */
/* (SG prime: p such that 2p+1 is also prime) */

#include <stddef.h>           /* for size_t */
#include "../utils/is_prime.h"

/* maximum number of SG primes we will store */
#define MAX_STORED 20000

static seq_t      cumulative[MAX_STORED];   /* cumulative sums */
static size_t    stored = 0;                /* number of SG primes stored */
static idx_t     candidate = 2;             /* next integer to test */
static seq_t     curr_sum = 0;              /* running sum of SG primes */

/* Grow the list of SG primes until we have at least 'need' of them */
static void generate_sg_primes(idx_t need)
{
    while (stored < need) {
        idx_t p = candidate;                      /* candidate prime */
        /* test p and 2p+1 for primality */
        if (is_prime(p) && is_prime(p * 2 + 1)) {
            /* add p to the running sum and store it */
            curr_sum = curr_sum + (seq_t)p;
            cumulative[stored] = curr_sum;
            ++stored;
        }
        /* advance candidate: after 2 go to 3, then skip evens */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

/* Public accessor required by the OEIS harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */
    if (stored < (size_t)n) {
        generate_sg_primes(n);                /* ensure enough primes */
    }
    return cumulative[(size_t)n - 1];
}
