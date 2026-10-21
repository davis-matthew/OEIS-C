/*
 * A060874  Intrinsic 4-palindromes: n is an intrinsic k-palindrome if it is a
 *          k-digit palindrome in some base.
 * https://oeis.org/A060874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060874: intrinsic 4‑palindromes.
   a(n) is the nth smallest integer that can be written as a 4‑digit palindrome
   (digits d3d2d2d3) in some base b≥2.  The values are generated
   by enumerating all possibilities a*(b³+1)+c*(b²+b) with
   1≤a<b and 0≤c<b, sorting and removing duplicates. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparison routine for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Global cache of the sequence */
static seq_t  *terms        = NULL;
static size_t  terms_len    = 0;
static bool    initialized  = false;

/* Build the cache once.  150 bases give more than 1 million raw terms,
   which after deduplication is far more than needed for n≤10000. */
static void init_terms(void)
{
    const idx_t MAX_BASE = 150;
    size_t estimated = (size_t)MAX_BASE * (MAX_BASE + 1) * (MAX_BASE - 1) / 3;
    terms = (seq_t *)malloc(estimated * sizeof(seq_t));
    if (!terms) exit(1);          /* allocation failure */

    terms_len = 0;
    for (idx_t b = 2; b <= MAX_BASE; ++b) {
        seq_t bSeq = (seq_t)b;
        seq_t b3   = bSeq * bSeq * bSeq;
        seq_t base = bSeq * (bSeq + (seq_t)1);   /* b² + b */

        for (idx_t a = 1; a < b; ++a) {
            seq_t part1 = (seq_t)a * (b3 + (seq_t)1);
            for (idx_t c = 0; c < b; ++c) {
                terms[terms_len++] = part1 + (seq_t)c * base;
            }
        }
    }

    qsort(terms, terms_len, sizeof(seq_t), seq_cmp);

    size_t uniq_len = 0;
    for (size_t i = 0; i < terms_len; ++i) {
        if (i == 0 || seq_cmp(&terms[i], &terms[uniq_len - 1]) != 0) {
            terms[uniq_len++] = terms[i];
        }
    }
    terms_len = uniq_len;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    if (!initialized) init_terms();
    if (n > terms_len) return 0;   /* out of range for the requested limit */
    return terms[n - 1];
}
