/*
 * A174228  Divisors of 24!.
 * https://oeis.org/A174228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy
 * Formula:   a(n) = the n-th divisor of 24! in increasing order, n ≥ 1. The
 *            total number of divisors is 242880. This routine generates all
 *            divisors on the first call, sorts them, and returns the required
 *            term
 * Range:     u8 none, u16 none, u32 n <= 3463, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compare_seq_t.h"   /* compare_seq_t defined here */

/* static storage for the divisor sequence */
static seq_t *divs = NULL;          /* array of all divisors of 24!   */
static size_t div_count = 242880;   /* number of divisors           */
static bool ready = false;          /* true once the array is ready  */

/* ------------------------------------------------------------ */
/* Recursive generator of divisors of 24!                        */
/* ------------------------------------------------------------ */
static void generate_divs(int prime_idx, seq_t current,
                          const int primes[], const int exps[])
{
    if (prime_idx == 9) {
        /* store the divisor */
        static size_t pos = 0;
        divs[pos++] = current;
        return;
    }

    int p = primes[prime_idx];
    seq_t val = current;

    /* exponent 0 */
    generate_divs(prime_idx + 1, val, primes, exps);

    /* exponents 1 .. exps[prime_idx] */
    for (int i = 1; i <= exps[prime_idx]; ++i) {
        val *= (seq_t)p;                 /* multiplication may wrap */
        generate_divs(prime_idx + 1, val, primes, exps);
    }
}

/* ------------------------------------------------------------ */
/* Public function                                               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!ready) {
        /* list of primes dividing 24! and their exponents */
        static const int primes[9] = {2,3,5,7,11,13,17,19,23};
        static const int exps[9]   = {22,10,4,3,2,1,1,1,1};

        /* allocate array large enough for all divisors */
        divs = (seq_t *)malloc(div_count * sizeof(seq_t));
        if (!divs) {
            /* allocation failure – return 0 as an error value */
            return (seq_t)0;
        }

        /* generate all divisors */
        generate_divs(0, (seq_t)1, primes, exps);

        /* sort the array (ascending) */
        qsort(divs, div_count, sizeof(seq_t), compare_seq_t);

        ready = true;
    }

    /* n is 1‑based; the array is 0‑based */
    return divs[(size_t)(n - 1)];
}
