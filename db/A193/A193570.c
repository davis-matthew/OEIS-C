/*
 * A193570  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 145.
 * https://oeis.org/A193570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer (in increasing order) that is an unhappy
 *            number whose chain of sum‑of‑squares of decimal digits first
 *            reaches 145 in the unhappy cycle (4, 16, 37, 58, 89, 145, 42,
 *            20)
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute sum of squares of decimal digits of x. Works for any seq_t
   that supports the operators % and / with 10 and multiplication. */
static seq_t sumsq_digits(seq_t x)
{
    seq_t s = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        s += (seq_t)d * (seq_t)d;
        x /= 10;
    }
    return s;
}

/* Return true iff the chain starting at x is unhappy and first
   encounters 145 before any other member of the unhappy cycle. */
static bool hits_145_first(seq_t x)
{
    /* Handle special cycle members that would terminate the chain
       before 145. */
    if (x == (seq_t)4 || x == (seq_t)16 || x == (seq_t)37 ||
        x == (seq_t)58 || x == (seq_t)89 || x == (seq_t)42 ||
        x == (seq_t)20) return false;
    if (x == (seq_t)145) return true;

    seq_t y = x;
    while (true) {
        y = sumsq_digits(y);
        if (y == (seq_t)145) return true;
        if (y == (seq_t)4 || y == (seq_t)16 || y == (seq_t)37 ||
            y == (seq_t)58 || y == (seq_t)89 || y == (seq_t)42 ||
            y == (seq_t)20) return false;
        if (y == (seq_t)1) return false;          /* happy number */
    }
}

/* Dynamic array for already found terms. */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_capacity = 0;
static seq_t next_candidate = 1;

/* Ensure that `terms` has at least `min_cap` elements available. */
static void ensure_terms_capacity(size_t min_cap)
{
    if (terms_capacity >= min_cap) return;
    size_t new_cap = terms_capacity ? terms_capacity * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* unrecoverable allocation failure */
    terms = tmp;
    terms_capacity = new_cap;
}

/* Public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index; return 0 safely */

    /* Generate terms until we have at least n of them. */
    while (terms_count < (size_t)n) {
        seq_t x = next_candidate++;
        if (hits_145_first(x)) {
            ensure_terms_capacity(terms_count + 1);
            terms[terms_count++] = x;
        }
    }
    return terms[n - 1];
}
