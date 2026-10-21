/*
 * A093139  Primes of the form 10*R_k + 7, where R_k is the repunit (A002275)
 *          of length k.
 * https://oeis.org/A093139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A093139: primes of the form 10*R_k+7, where R_k is the repunit of length k (A002275). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;      /* cached primes */
    static size_t val_cap = 0;
    static size_t val_len = 0;
    static seq_t repunit = 0;       /* current repunit R_k */

    while (val_len < (size_t)n) {
        if (val_len >= val_cap) {
            size_t new_cap = val_cap ? val_cap * 2 : 8;
            void *tmp = realloc(vals, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            vals = (seq_t *)tmp;
            val_cap = new_cap;
        }

        /* candidate = 10*repunit + 7 */
        seq_t cand = mul_small(repunit, (unsigned)10);
        cand += (unsigned)7;

        if (is_prime_big(cand)) {
            vals[val_len++] = cand;
        }

        /* advance to next repunit: R_{k+1} = R_k*10 + 1 */
        repunit = mul_small(repunit, (unsigned)10);
        repunit += (unsigned)1;
    }

    return vals[n - 1];
}
