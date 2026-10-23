/*
 * A136340  Fibonacci numbers where every digit is a prime.
 * https://oeis.org/A136340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  Compute A136340 by iterating Fibonacci numbers and
 * collecting those whose decimal digits are all prime digits
 * (2, 3, 5, or 7).  No closed‑form formula is known, so a search
 * is performed lazily with memoization so that arbitrary calls to
 * a(n) are handled efficiently. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

extern seq_t fib(idx_t n);          /* provided utility; not used directly here */

/* Helper: check if all decimal digits of x are prime digits. */
static bool all_prime_digits(seq_t x)
{
    const seq_t TEN = (seq_t)10;
    const seq_t TWO = (seq_t)2;
    const seq_t THREE = (seq_t)3;
    const seq_t FIVE = (seq_t)5;
    const seq_t SEVEN = (seq_t)7;

    while (x != (seq_t)0) {
        seq_t d = x % TEN;
        if (d != TWO && d != THREE && d != FIVE && d != SEVEN)
            return false;
        x /= TEN;
    }
    return true;
}

/* The function required by the framework. */
seq_t a(idx_t n)
{
    /* Lazy initialization of static state. */
    static bool ready = false;
    static seq_t *terms = NULL;     /* array of found terms */
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    /* Fibonacci state for generating terms on demand. */
    static seq_t f_prev = (seq_t)0; /* F(i-1) */
    static seq_t f_curr = (seq_t)1; /* F(i)   */
    static idx_t fib_idx = 2;       /* current index of f_curr (F(2) initially) */

    if (!ready) {
        /* Initialize first known terms if any were hard coded here. */
        /* We start with the standard Fibonacci seed: F(1)=1, F(2)=1. */
        f_prev = (seq_t)1;
        f_curr = (seq_t)1;
        fib_idx = 2;
        ready = true;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        /* Compute next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
        ++fib_idx;          /* now f_curr corresponds to F(fib_idx) */

        if (all_prime_digits(f_curr)) {
            /* Grow terms array if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out of memory: return 0 */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = f_curr;
        }
    }

    return terms[n - 1];
}
