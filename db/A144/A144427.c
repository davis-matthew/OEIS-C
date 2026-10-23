/*
 * A144427  Primes of the form (prime(k) + (-1)^k*3)/2.
 * https://oeis.org/A144427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime of the form (prime(k) + (-1)^k * 3) / 2
 *            where k is the n-th index for which that value is prime
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Compute the n-th term of OEIS sequence A144427. */
seq_t a(idx_t n)
{
    /* Guard against an invalid index. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static storage for computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_k = 2;   /* start from k = 2 (k = 1 is not used) */

    /* Ensure capacity for new terms. */
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 128 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe fallback. */
            return (seq_t)0;
        }
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        /* Get the k‑th prime (prime indices are 1‑based). */
        uint32_t pk = get_prime((idx_t)(next_k));
        seq_t val = (seq_t)pk;

        /* Apply the +/-3/two pattern. */
        if ((next_k & 1U) == 0) {
            val += (seq_t)3;
        } else {
            val -= (seq_t)3;
        }
        val /= (seq_t)2;

        /* Test if the resulting value is prime. */
        if (is_prime_big(val)) {
            terms[terms_len++] = val;
        }

        ++next_k;
    }

    return terms[n - 1];
}
