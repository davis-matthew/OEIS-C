/*
 * A235811  Numbers k such that k^3 has one or more occurrences of exactly
 *          nine different digits.
 * https://oeis.org/A235811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235811: numbers k such that k^3 has exactly nine distinct decimal digits.
   The implementation generates the sequence by brute‑force search,
   memoising all found terms so that any order of calls to a() is fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t *terms    = NULL;   /* memoised output terms */
static size_t terms_sz = 0;      /* number of terms stored     */
static size_t terms_cap = 0;     /* array capacity             */
static seq_t next_k = 1;         /* next candidate k to test   */

/* append a new term, growing the array if necessary */
static void append_term(seq_t val)
{
    if (terms_sz == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
        seq_t *ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!ptr) abort();          /* allocation failed */
        terms = ptr;
        terms_cap = new_cap;
    }
    terms[terms_sz++] = val;
}

/* count distinct decimal digits in x */
static int distinct_digits(seq_t x)
{
    bool seen[10] = { false };
    int cnt = 0;
    if (x == 0) return 1;          /* only digit 0 */

    while (x > 0) {
        int d = (int)(x % 10);
        if (!seen[d]) {
            seen[d] = true;
            if (++cnt > 9) return cnt;  /* early exit */
        }
        x /= 10;
    }
    return cnt;
}

/* a(n) returns the nth term (n >= 1) of A235811 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* generate terms until we have at least n of them */
    while (terms_sz < (size_t)n) {
        seq_t k = next_k++;
        seq_t cube = big_pow(k, (seq_t)3);
        if (distinct_digits(cube) == 9)
            append_term(k);
    }
    return terms[n - 1];
}
