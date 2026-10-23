/*
 * A152607  a(1) = 1; thereafter a(n) is always the smallest integer > a(n-1)
 *          not leading to a contradiction, such that the concatenation of any
 *          two consecutive digits in the sequence is a prime.
 * https://oeis.org/A152607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, a(n) = smallest integer > a(n‑1) such that *
 *            the concatenation of any two consecutive digits in the entire *
 *            digit sequence is a prime
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static bool adjacency[10][10];
static bool adj_inited = false;

static void init_adjacency(void)
{
    if (adj_inited) return;
    for (int d1 = 0; d1 <= 9; ++d1)
        for (int d2 = 0; d2 <= 9; ++d2)
            adjacency[d1][d2] = is_prime_small(d1 * 10 + d2);
    adj_inited = true;
}

/* test whether cand can follow a sequence whose last digit is last_digit */
static bool candidate_valid(seq_t cand, unsigned char last_digit)
{
    /* extract digits in reverse order */
    int digits[256];
    int len = 0;
    seq_t tmp = cand;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }
    if (len == 0) return false;          /* should never happen */

    unsigned char first = (unsigned char)digits[len - 1];
    if (!adjacency[last_digit][first]) return false;

    for (int i = len - 1; i > 0; --i) {
        unsigned char d1 = (unsigned char)digits[i];
        unsigned char d2 = (unsigned char)digits[i - 1];
        if (!adjacency[d1][d2]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t terms_size = 0;   /* number of terms stored */
    static idx_t capacity   = 0;   /* allocated capacity */

    init_adjacency();

    if (n < 1) return 0;          /* offset starts at 1 */

    /* initial allocation */
    if (capacity == 0) {
        capacity = 64;
        terms = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!terms) return 0;
    }

    /* generate terms up to requested n */
    while (terms_size < n) {
        if (terms_size == 0) {          /* first term */
            terms[0] = (seq_t)1;
            terms_size = 1;
            continue;
        }

        /* grow storage if needed */
        if (terms_size == (size_t)capacity) {
            capacity <<= 1;
            terms = (seq_t*)realloc(terms, sizeof(seq_t) * capacity);
            if (!terms) return 0;
        }

        /* find next term */
        seq_t candidate = terms[terms_size - 1] + (seq_t)1;
        unsigned char last_digit = (unsigned char)(terms[terms_size - 1] % 10);

        while (!candidate_valid(candidate, last_digit))
            candidate += (seq_t)1;

        terms[terms_size++] = candidate;
    }

    return terms[n - 1];
}
