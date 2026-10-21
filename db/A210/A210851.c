/*
 * A210851  Digits of one of the two 5-adic integers sqrt(-1).
 * https://oeis.org/A210851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210851 – digits of one of the two 5‑adic integers sqrt(-1).
   a(0) = 3.  For n ≥ 1 the following recurrence is used:

   carry(1)           = (a(0)² + 1) / 5
   sum_i              = Σ_{k=1}^{i-1} a(k) · a(i-k)
   digit_i            = (sum_i + carry_i) mod 5
   a(i)                = (5 - digit_i) mod 5
   carry_{i+1} = (sum_i + 2·a(0)·a(i) + carry_i) / 5

   The function keeps a memoised vector of already‑computed digits so
   that any call can be answered in O(1) after at most one linear pass
   to extend the cache.  All arithmetic is done in the type seq_t; the
   code does not rely on any helper functions beyond the C standard
   library. */

#include <stdlib.h>   /* malloc, realloc */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache of computed digits, their capacity and current length.
       carry holds the carry entered at the current position. */
    static seq_t *digits = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t carry = 0;

    /* Initialise on first call */
    if (digits == NULL) {
        cap = 1;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!digits) abort();            /* out‑of‑memory */
        digits[0] = (seq_t)3;            /* a(0) = 3 */
        len = 1;
        /* carry(1) = (a(0)^2 + 1) / 5  →  (9 + 1) / 5 = 2 */
        carry = ((seq_t)3 * (seq_t)3 + (seq_t)1) / (seq_t)5;
    }

    /* Extend cache up to the requested index n */
    size_t ns = (size_t)n;
    while (len <= ns) {
        size_t i = len;                  /* compute a(i) */

        /* sum_i = Σ_{k=1}^{i-1} a(k)·a(i‑k) */
        seq_t sum = 0;
        for (size_t k = 1; k < i; ++k) {
            sum += digits[k] * digits[i - k];
        }

        /* digit_i = (sum + carry) mod 5 */
        seq_t digit = (sum + carry) % (seq_t)5;

        /* a(i) = (5 - digit) mod 5 */
        seq_t ai = ((seq_t)5 - digit) % (seq_t)5;

        /* carry_{i+1} = (sum + 2·a(0)·ai + carry) / 5 */
        carry = (sum + (seq_t)2 * digits[0] * ai + carry) / (seq_t)5;

        /* Grow the array if necessary */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out‑of‑memory */
            digits = tmp;
            cap = new_cap;
        }

        digits[len] = ai;
        ++len;
    }

    return digits[ns];
}
