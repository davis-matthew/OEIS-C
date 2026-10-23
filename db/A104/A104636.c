/*
 * A104636  Even n such that 2n+1 is prime.
 * https://oeis.org/A104636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth even integer n such that 2n+1 is prime
 *            (equivalently, n = (q-1)/2 for the nth prime q ≡ 1 (mod 4))
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *terms = NULL;          /* storage for computed terms          */
static idx_t  terms_capacity = 0;    /* current allocated capacity          */
static idx_t  terms_size = 0;        /* number of terms stored             */
static uint64_t next_candidate = 5;  /* first odd prime > 3 (q ≡ 1 (mod 4)) */

static void ensure_capacity(idx_t needed)
{
    if (terms_capacity >= needed)
        return;
    idx_t new_cap = (terms_capacity == 0) ? 128 : terms_capacity * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(*terms));
    if (!tmp)
        abort();                     /* allocation failure: terminate */
    terms = tmp;
    terms_capacity = new_cap;
}

/* Extend the terms array to contain at least 'up_to' elements. */
static void extend_terms(idx_t up_to)
{
    ensure_capacity(up_to);
    while (terms_size < up_to) {
        /* Search for the next prime q ≡ 1 (mod 4). */
        for (;;) {
            if ((next_candidate & 3U) == 1U && is_prime(next_candidate)) {
                /* q is prime and ≡1 mod4 → n = (q-1)/2 is even. */
                seq_t val = (seq_t)((next_candidate - 1) / 2);
                terms[terms_size++] = val;
                break;
            }
            next_candidate += 2;      /* skip to next odd candidate   */
        }
        /* Prepare the next search start (ensure we skip the prime we just used). */
        next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for 0, guard against misuse */

    if (terms_size < n)
        extend_terms(n);

    return terms[n - 1];              /* a(1) is the first stored element   */
}
