/*
 * A052490  Numbers k such that there is exactly one number which is k times
 *          the sum of its digits.
 * https://oeis.org/A052490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A052490: numbers k such that there is exactly one positive
 *  integer n with n = k * sum_of_digits(n).
 *  The function builds the sequence on demand by searching sequentially
 *  for k that have a unique solution.  Results are memoised in a static
 *  array so any order of calls works efficiently.  The code
 *  follows C11 / C++17 rules and uses only the allowed utilities.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility header providing digit_sum(seq_t) and digit_len(seq_t). */
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"

/* -- static state for memoisation ------------------------------------- */

/* Cached terms of the sequence */
static seq_t *terms = NULL;
/* Number of terms currently stored */
static idx_t term_count = 0;
/* Allocated capacity of 'terms' */
static idx_t capacity   = 0;
/* Largest k that has already been examined */
static seq_t max_tested = 0;

/* -- helper ------------------------------------------------------------ */

/* Determine whether a given k has *exactly one* solution
 * n = k * digit_sum(n).  */
static bool has_one_solution(seq_t k)
{
    idx_t count = 0;
    for (idx_t S = 1; ; ++S) {
        /* Candidate n = k * S */
        seq_t n = k * (seq_t)S;

        /* Number of decimal digits of n */
        idx_t d = (idx_t)digit_len(n);

        /* If S exceeds the maximum possible digit sum for n, stop. */
        if (S > 9 * d) break;

        /* Digit sum of n */
        seq_t sd = digit_sum(n);
        if (sd == (seq_t)S) {
            ++count;
            if (count > 1) return false;   /* more than one solution found */
        }
    }
    return count == 1;   /* exactly one solution */
}

/* Grow the capacity of the terms array if necessary. */
static void ensure_capacity(idx_t new_cap)
{
    if (capacity >= new_cap) return;
    idx_t cap = capacity == 0 ? 16 : capacity;
    while (cap < new_cap) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
    terms = tmp;
    capacity = cap;
}

/* -- required function ------------------------------------------------- */

/* Return a(n) – the n-th element of A052490; n ≥ 1.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        ++max_tested;
        if (has_one_solution(max_tested)) {
            ensure_capacity(term_count + 1);
            terms[term_count++] = max_tested;
        }
    }
    return terms[n - 1];
}
