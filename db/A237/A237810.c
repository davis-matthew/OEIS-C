/*
 * A237810  Primes p such that 2*p+1 and 2*p+7 are also prime.
 * https://oeis.org/A237810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 217, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237810
   Primes p such that 2*p+1 and 2*p+7 are also prime.
   Search based implementation – the function returns the nth such prime
   (1‑based index).  It caches discovered terms for repeated calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *answers = NULL;          /* cached sequence terms */
static size_t  ans_len  = 0;           /* number of stored terms    */
static size_t  ans_cap  = 0;           /* current capacity of array */
static idx_t   last_cand = 1;          /* last integer tested       */

/* Ensure the answers array can accommodate at least 'need' terms. */
static void ensure_ans_cap(size_t need)
{
    if (need <= ans_cap) return;
    size_t new_cap = ans_cap ? ans_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* very unlikely */
    answers = tmp;
    ans_cap  = new_cap;
}

/* Generate terms until at least 'n' terms are cached.  */
static void grow_to(idx_t n)
{
    while (ans_len < (size_t)n) {
        idx_t p = ++last_cand;                /* next candidate prime */
        if (is_prime(p) &&
            is_prime((idx_t)(p << 1) + 1) &&
            is_prime((idx_t)(p << 1) + 7)) {
            ensure_ans_cap(ans_len + 1);
            answers[ans_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; 0 is unused */
    grow_to(n);
    return answers[n - 1];
}
