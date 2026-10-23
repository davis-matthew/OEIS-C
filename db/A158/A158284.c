/*
 * A158284  List of pairs p, p+2 where p is prime and p and p+2 contain the
 *          same number of prime digits.
 * https://oeis.org/A158284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158284: List of pairs p, p+2 where p is prime and p and p+2 have the
   same number of prime digits (2, 3, 5, 7). Each qualifying twin prime pair
   contributes two consecutive terms in the sequence. The function enumerates
   twin prime pairs until it has generated at least n terms and then returns
   the n-th term (1‑based).  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static inline bool is_prime_digit(unsigned d)
{
    return d == 2 || d == 3 || d == 5 || d == 7;
}

static uint32_t count_prime_digits(uint64_t n)
{
    uint32_t cnt = 0;
    if (n == 0) return 0;
    while (n) {
        unsigned d = (unsigned)(n % 10u);
        if (is_prime_digit(d)) ++cnt;
        n /= 10u;
    }
    return cnt;
}

/* Dynamic array of results.  */
static seq_t *terms = NULL;     /* array of terms generated so far */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of the array */
static idx_t next_candidate = 2; /* next prime to test */

/* Ensure the terms array can hold at least `need` elements */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t newcap = terms_cap ? terms_cap * 2 : 64;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = newcap;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset */

    /* On first call initialise static storage. */
    if (!terms) {
        terms = NULL;
        terms_len = 0;
        terms_cap = 0;
        next_candidate = 2;
    }

    /* Generate results until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Find next twin prime pair that satisfies the digit condition */
        while (true) {
            if (is_prime(next_candidate)) {
                uint32_t c1 = count_prime_digits((uint64_t)next_candidate);
                uint64_t q = next_candidate + 2u;
                uint32_t c2 = count_prime_digits(q);
                if (c1 == c2) {
                    /* Store the pair (p , p+2). */
                    grow_terms(terms_len + 2);
                    terms[terms_len++] = (seq_t)next_candidate;
                    terms[terms_len++] = (seq_t)q;
                    /* Continue searching from next integer. */
                    ++next_candidate;
                    break;
                }
            }
            ++next_candidate;
        }
    }

    return terms[(size_t)n - 1];
}
