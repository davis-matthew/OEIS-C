/*
 * A104322  Primes which are the reverse concatenation of five consecutive
 *          cubes.
 * https://oeis.org/A104322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digit_len, isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A104322: primes that are the reverse concatenation of five
    consecutive cubes.
    For a given starting integer k ≥ 1 we construct the string
    (k+4)^3 · (k+3)^3 · (k+2)^3 · (k+1)^3 · k^3
    where · denotes decimal concatenation (the cubes are written in
    normal decimal order but the whole block is concatenated in reverse
    order).  The resulting integer is tested for primality; all such
    primes are listed in increasing order of k.  a(1) is the first
    prime produced in this way.  The sequence is indexed starting at 1. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/isprime_big.h"

seq_t a(idx_t n)
{
    /* The first few terms grow very rapidly; we keep a static cache
       and generate terms on demand.  The cache is big enough to hold
       at least 10000 terms for the purposes of the tests. */
    static seq_t  terms[200000];
    static idx_t  size      = 0;   /* number of terms stored          */
    static idx_t  last_k    = 0;   /* last starting cube index used   */

    if (n == 0) return 0;          /* the sequence is 1‑based */

    /* Generate primes until we have at least n of them.  */
    while (size < n) {
        ++last_k;                                 /* current k      */
        seq_t k = (seq_t)last_k;

        /* Compute the five consecutive cubes. */
        seq_t cube[5];
        for (int i = 0; i < 5; ++i) {
            seq_t x = k + (seq_t)i;
            cube[i] = x * x * x;
        }

        /* Build the concatenation in reverse cube order:
           (k+4)^3 · ... · k^3. */
        seq_t concat = 0;
        for (int idx = 4; idx >= 0; --idx) {
            unsigned len = (unsigned)digit_len(cube[idx]); /* number of decimal digits */
            concat = concat * pow10(len) + cube[idx];
        }

        /* Test for primality and store if prime. */
        if (isprime_big(concat)) {
            terms[size++] = concat;
        }
    }

    return terms[n - 1];
}
