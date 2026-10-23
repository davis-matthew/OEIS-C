/*
 * A045548  Numbers whose factorial has '4' as its final nonzero digit.
 * https://oeis.org/A045548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) – numbers whose factorial has 4 as its final non‑zero digit
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches sequentially, maintaining the last non‑zero
   digit of n! modulo a large power of 10. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static storage for the sequence terms. */
static seq_t *terms       = NULL;
static size_t terms_cnt   = 0;
static size_t terms_cap   = 0;
static uint64_t last_processed = 0;
static uint64_t acc       = 1;

/* Grow the terms array if necessary (works for both C and C++). */
static void ensure_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_arr = new (std::nothrow) seq_t[new_cap];
    if (!new_arr) abort();
    for (size_t i = 0; i < terms_cnt; ++i)
        new_arr[i] = terms[i];
    delete[] terms;
    terms = new_arr;
#else
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    terms = new_arr;
#endif
    terms_cap = new_cap;
}

/* Return the nth term of A045548 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, but safe */

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        ++last_processed;
        uint64_t i = last_processed;

        /* Update the last‑non‑zero digit of i! */
        acc *= i;
        while ((acc % 10ULL) == 0ULL)
            acc /= 10ULL;
        acc %= 1000000000ULL;   /* keep the value small */

        if ((acc % 10ULL) == 4ULL) {
            /* Store the term */
            ensure_cap(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)last_processed;
        }
    }

    return terms[n - 1];
}
