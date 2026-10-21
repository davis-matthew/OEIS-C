/*
 * A065816  Numbers k such that the alternating sum of digits of k^2 is 0.
 * https://oeis.org/A065816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) is the n-th integer k whose square has alternating sum of
 *            digits equal to 0. The alternating sum is taken from the most
 *            significant digit, adding and subtracting alternately
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *terms  = NULL;  /* cache of found terms           */
static size_t terms_len = 0;  /* number of terms stored so far   */
static size_t terms_cap = 0;  /* allocated capacity              */
static idx_t next_candidate = 1;  /* next integer to check */

/* grow the terms array to at least 'need' elements */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* append a new term to the cache */
static void add_term(idx_t val)
{
    if (terms_len == terms_cap)
        grow_terms(terms_len + 1);
    terms[terms_len++] = (seq_t)val;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined by OEIS, return 0 for safety */

    /* generate more terms if needed */
    while (terms_len < (size_t)n)
    {
        /* compute k^2 in a type that can hold the product   */
        unsigned __int128 sq = (unsigned __int128)next_candidate * next_candidate;

        /* alternating sum of digits, most significant first */
        int dig[40];          /* sufficient for 128‑bit numbers */
        int dcnt = 0;
        while (sq > 0)
        {
            dig[dcnt++] = (int)(sq % 10);
            sq /= 10;
        }
        int sum = 0;
        int sign = 1;
        for (int i = dcnt - 1; i >= 0; --i)
        {
            sum += sign * dig[i];
            sign = -sign;
        }

        if (sum == 0)
            add_term(next_candidate);

        ++next_candidate;
    }

    return terms[n - 1];
}
