/*
 * A117124  Numbers that when multiplied by 37 produce a palindrome number.
 * https://oeis.org/A117124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: numbers k such that 37*k is a decimal palindrome (searched). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static seq_t *cache = NULL;   /* stored terms */
static size_t cap = 0;        /* allocated capacity */
static size_t nterms = 0;     /* number of terms stored */
static idx_t next_checked = 1;/* next integer to test */

/* Grow the cache to at least 'new_cap' elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap) return;
    size_t new_cap2 = (cap == 0) ? 8 : cap * 2;
    while (new_cap2 < new_cap) new_cap2 <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cap = new_cap2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – undefined for n<1 */

    /* Generate terms until we have at least n entries. */
    while (nterms < n) {
        /* Find the next palindrome multiplier. */
        for (;;) {
            seq_t cand = (seq_t)next_checked;
            seq_t prod = cand * (seq_t)37;

            if (is_palindrome(prod, (idx_t)10)) {
                /* Store the candidate. */
                ensure_capacity(nterms + 1);
                cache[nterms++] = cand;
                ++next_checked; /* Move past this candidate. */
                break;
            }
            ++next_checked;
        }
    }
    return cache[n - 1];
}
