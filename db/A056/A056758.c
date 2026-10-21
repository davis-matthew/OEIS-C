/*
 * A056758  Numbers n for which d(n)^3 < n.
 * https://oeis.org/A056758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A056758: numbers n with d(n)^3 < n
 * using an incremental divisor‑sieve and on‑demand sequence storage.
 * The function returns the n‑th term (1‑based) and works for any
 * implementation of seq_t (8, 16, 32, 64, 128 bits or arbitrary precision).
 * The implementation is fully compliant with C11 and C++17.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state ----------------------------------------------------------- */
static seq_t   *seq  = NULL;   /* sequence values           */
static size_t   seq_len = 0;   /* number of terms stored    */
static size_t   seq_cap = 0;   /* capacity of seq[]         */
static idx_t    seq_max_n = 0; /* highest n processed for seq */

static uint32_t *divcnt = NULL;     /* divisor counts for 1..limit  */
static size_t    divcnt_cap = 0;    /* capacity of divcnt[]         */
static idx_t     div_limit = 0;     /* highest n processed for divcnt */

/* Append a term to the sequence array ----------------------------------- */
static void append_term(seq_t value)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }
    seq[seq_len++] = value;
}

/* Extend the divisor count array up to new_limit (increasing only)------- */
static void extend_divcnt(idx_t new_limit)
{
    /* Grow the array if needed */
    if (divcnt_cap <= (size_t)new_limit) {
        size_t new_cap = (size_t)new_limit + 1;        /* index 0 unused */
        uint32_t *tmp = (uint32_t *)realloc(divcnt, new_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        divcnt = tmp;
        divcnt_cap = new_cap;
    }

    /* Initialize new entries to 0 */
    for (idx_t i = div_limit + 1; i <= new_limit; ++i)
        divcnt[i] = 0;

    /* Update divisor counts incrementally */
    for (idx_t i = 1; i <= new_limit; ++i) {
        /* Find first multiple of i that is > div_limit */
        idx_t start = (div_limit / i + 1) * i;
        if (start <= div_limit) start = i;   /* for i > div_limit this is i itself */
        for (idx_t j = start; j <= new_limit; j += i)
            ++divcnt[j];
    }

    div_limit = new_limit;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe to return 0 */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Increase the sieving limit exponentially */
        idx_t new_limit = seq_max_n ? seq_max_n * 2 : 2;
        extend_divcnt(new_limit);

        /* Append new qualifying terms */
        for (idx_t k = seq_max_n + 1; k <= new_limit; ++k) {
            uint32_t d = divcnt[k];
            seq_t d3 = (seq_t)d * d * d;
            if (d3 < (seq_t)k)
                append_term((seq_t)k);
        }
        seq_max_n = new_limit;
    }

    return seq[n - 1];
}
