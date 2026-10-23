/*
 * A166975  Four-digit primes such that, if the digits are ABCD, then AB+CD
 *          and A+B+C+D are also primes.
 * https://oeis.org/A166975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 179, u32 n <= 179, u64 n <= 179, u128 n <= 179,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute all 4‑digit primes ABCD such that AB+CD and A+B+C+D are primes */

/* Utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Store the sequence once; read‑only after initialization. */
static seq_t *terms = NULL;
static size_t terms_len = 0;

/* Initialise the sequence the first time a() is called. */
static void init_terms(void)
{
    /* Compute all 4‑digit primes; there are only 179 such primes. */
    const size_t CAP = 200;          /* enough space for all terms */
    terms = (seq_t *)malloc(CAP * sizeof(seq_t));
    if (!terms) return;             /* allocation failed – give up */

    size_t count = 0;
    for (int n = 1000; n <= 9999; ++n) {
        if (!is_prime_small((seq_t)n)) continue;

        int A = n / 1000;
        int B = (n / 100) % 10;
        int C = (n / 10) % 10;
        int D = n % 10;

        int digit_sum = A + B + C + D;        /* A+B+C+D */
        int ab = 10 * A + B;                  /* AB */
        int cd = 10 * C + D;                  /* CD */

        if (is_prime_small((seq_t)digit_sum) &&
            is_prime_small((seq_t)(ab + cd))) {
            if (count < CAP) {
                terms[count++] = (seq_t)n;
            }
        }
    }
    terms_len = count;
}

/* Return the n-th term (n >= 1).  If n exceeds the number of terms,
 * return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* offset starts at 1 */

    if (!terms) init_terms();                     /* initialise once */

    if (n > (idx_t)terms_len) return 0;           /* no such term */
    return terms[(size_t)(n - 1)];
}
