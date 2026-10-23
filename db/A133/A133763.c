/*
 * A133763  Odd composite numbers such that the sum of any two terms, plus 1,
 *          is composite.
 * https://oeis.org/A133763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A133763: odd composite numbers such that the sum of any two terms plus 1 is composite.
   Recurrence: a(1)=9; for n>1, a(n) is the smallest odd composite > a(n‑1) with
   a(n)+a(k)+1 composite for all k < n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;
static size_t len   = 0;
static size_t cap   = 0;

/* Ensure the array can hold at least `len + 1` terms. */
static void grow_terms(void)
{
    if (len + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < len + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    if (len == 0) {
        grow_terms();
        terms[0] = (seq_t)9;
        len = 1;
    }

    /* Generate terms until we reach the requested index. */
    while (len < (size_t)n) {
        seq_t cand = terms[len - 1] + (seq_t)2;   /* next odd candidate */

        while (true) {
            /* skip primes; candidate must be composite. */
            if (!is_prime_big(cand)) {
                bool ok = true;
                for (size_t j = 0; j < len; ++j) {
                    seq_t sum = cand + terms[j] + (seq_t)1;
                    if (is_prime_big(sum)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) break;                     /* valid term found */
            }
            cand += (seq_t)2;                     /* try next odd number */
        }

        grow_terms();
        terms[len++] = cand;
    }

    return terms[n - 1];
}
