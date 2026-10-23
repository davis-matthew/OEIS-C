/*
 * A044265  Numbers n such that string 1,5 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A044265.
   a(n) is the n-th integer such that the base‑9 representation of the
   integer contains the substring "15" while the base‑9 representation of
   the previous integer does not.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* (pre‑)declaration of required static storage so it can be used inside
   the helper functions. */
static seq_t    *terms      = NULL;   /* array of found terms    */
static size_t   terms_count = 0;      /* number of terms stored  */
static size_t   terms_cap   = 0;      /* current allocation size */
static seq_t    cur_candidate = 1;    /* next candidate to test */

/* Helper: test if the base‑9 representation of x contains the adjacent
   digits 1 followed by 5. */
static bool has15(seq_t x)
{
    if (x == 0)
        return false;
    seq_t v = x;
    unsigned int digits[512];
    size_t len = 0;
    while (v > 0) {
        digits[len++] = (unsigned int)(v % 9);
        v /= 9;
    }
    /* Digits are least‑significant first.  Scan each adjacent pair. */
    for (size_t i = 1; i < len; ++i)
        if (digits[i] == 1 && digits[i - 1] == 5)
            return true;
    return false;
}

/* Ensure that the term buffer can hold at least 'need' elements. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed; terminate with bad status (must not happen). */
        exit(1);
    }
    terms = tmp;
    terms_cap = new_cap;
}

/* Generate terms until we have at least 'n' of them.  */
static void generate_up_to(idx_t n)
{
    while (terms_count < (size_t)n) {
        if (has15(cur_candidate) && !has15(cur_candidate - 1)) {
            grow_terms(terms_count + 1);
            terms[terms_count++] = cur_candidate;
        }
        ++cur_candidate;
    }
}

/* External linkage function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0 – return 0 for safety */
    generate_up_to(n);
    return terms[n - 1];
}
