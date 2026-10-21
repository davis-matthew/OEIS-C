/*
 * A088287  Palindromes divisible by the number formed by their internal
 *          digits.
 * https://oeis.org/A088287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   reverse_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088287: Palindromes divisible by the number formed by their internal digits.
   The palindromes are generated in increasing order and tested for the property:
   let L be the length of the palindrome, M be the integer formed by the internal
   digits (exclude the first and last), then the palindrome N satisfies N % M == 0.
   The function memoises the results so that repeated calls are constant‑time after
   the first few terms.  The algorithm runs in O(n) time for the first n terms
   (n ≤ 10 000).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_uint.h"

/* *** Memoised components *** */
static seq_t *terms = NULL;        /* dynamic array of found terms */
static size_t term_count = 0;      /* number of terms stored */
static size_t term_cap = 0;        /* capacity of the array */

static bool pow10_init = false;
static seq_t pow10[20];            /* pow10[i] = 10^i */

/* Initialise pow10 array (once) */
static void init_pow10(void)
{
    if (pow10_init) return;
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * (seq_t)10;
    pow10_init = true;
}

/* Increase the capacity of the terms array */
static void grow_terms(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_terms;
    term_cap = new_cap;
}

/* *** State for sequential palindrome generation *** */
static bool palgen_init = false;
static size_t curr_len;        /* current palindrome length */
static size_t half_len;        /* number of digits in the first half (with middle if odd) */
static uint64_t half_start;    /* first half value to start */
static uint64_t half_end;      /* maximum half value for current length */
static uint64_t curr_half;     /* current half value being processed */

/* Initialise palindrome generator for length L = 3 */
static void init_palgen(void)
{
    curr_len   = 3;
    half_len   = (curr_len + 1) / 2;
    half_start = 1;
    for (size_t i = 1; i < half_len; ++i) half_start *= 10;        /* 10^(half_len-1) */
    half_end = half_start * 10 - 1;                                /* 10^half_len - 1 */
    curr_half = half_start;
    palgen_init = true;
}

/* Produce the next palindrome (in increasing numeric order).
   Outputs the palindrome value and its length.
   Returns true each time (the sequence is infinite). */
static bool next_palindrome(seq_t *out, size_t *len_out)
{
    if (!palgen_init) init_palgen();

    for (;;) {
        /* Build palindrome from curr_half */
        uint64_t rev_part = (curr_len & 1) ? reverse_uint(curr_half / 10) : reverse_uint(curr_half);
        size_t rev_len = curr_len - half_len;                      /* number of digits in rev_part */
        seq_t pal = (seq_t)curr_half * pow10[rev_len] + (seq_t)rev_part;

        *out = pal;
        *len_out = curr_len;

        /* Advance to next half */
        ++curr_half;
        if (curr_half > half_end) {
            /* Move to next palindrome length */
            ++curr_len;
            half_len = (curr_len + 1) / 2;
            half_start = 1;
            for (size_t i = 1; i < half_len; ++i) half_start *= 10;
            half_end = half_start * 10 - 1;
            curr_half = half_start;
        }
        return true;
    }
}

/* *** Main sequence function *** */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but return 0 */

    init_pow10();                 /* ensure pow10 array ready */

    /* Generate terms until we have at least n entries */
    while (term_count < (size_t)n) {
        seq_t pal;
        size_t len;
        next_palindrome(&pal, &len);

        seq_t M = (pal / (seq_t)10) % pow10[len - 2];
        if (M == (seq_t)0) continue;                 /* internal number zero: skip */

        if ((pal % M) == (seq_t)0) {                 /* satisfies condition */
            grow_terms(term_count + 1);
            terms[term_count++] = pal;
        }
    }

    return terms[n - 1];
}
