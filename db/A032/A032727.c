/*
 * A032727  Numbers k such that k prefixed by '2' and followed by '9' is
 *          prime.
 * https://oeis.org/A032727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the integer formed by prefixing
 *            '2' and suffixing '9' to k is prime. That integer is
 *            2*10^{digits(k)+1} + k*10 + 9
 * Range:     u8 n <= 2, u16 n <= 295, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* array of found k values */
static size_t cap = 0;           /* current capacity of cache */
static size_t cnt = 0;           /* number of stored terms */
static idx_t last_k = 0;         /* last k tested */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Return a(n) for n >= 1.  No defined behavior for n == 0. */
    if (n == 0) return 0;

    /* Allocate initial cache space if needed */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cnt < n) {
        ++last_k;                                   /* next k to test */

        /* Compute e = digits(k) + 1 */
        idx_t e = (idx_t)digits(last_k) + 1;

        /* Form the candidate number: 2*10^e + k*10 + 9 */
        seq_t candidate = 2 * pow10(e) + ((seq_t)last_k * 10) + 9;

        /* Check primality */
        if (is_prime_big(candidate)) {
            /* Grow cache if necessary */
            if (cnt >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cnt++] = last_k;
        }
    }

    /* Return the cached value */
    return cache[n - 1];
}
