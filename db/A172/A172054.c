/*
 * A172054  n-th number k such that 6*k-1 is composite while 6*k+1 is prime
 *          minus n-th number m such that 6*m-1 is prime while 6*m+1 is
 *          composite.
 * https://oeis.org/A172054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2504, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172054: a(n) = A121765(n) – A121763(n)
   where A121765(n) is the n‑th k such that 6k‑1 is composite and
   6k+1 is prime, and A121763(n) is the n‑th m such that 6m‑1 is
   prime and 6m+1 is composite. */

/* The function generates the two auxiliary sequences on demand
   and then returns their difference.  Both sequences are stored
   in static arrays that grow as needed.  The algorithm is
   incremental – it keeps track of the next k and m to test – so
   multiple calls with arbitrary n are handled correctly
   regardless of call order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *K = NULL;          /* A121765 values (k)   */
static size_t cap_K = 0;
static size_t sz_K = 0;
static idx_t next_k = 1;         /* next k to test       */

static seq_t *M = NULL;          /* A121763 values (m)   */
static size_t cap_M = 0;
static size_t sz_M = 0;
static idx_t next_m = 1;         /* next m to test       */

/* Append a value to a dynamically growing array. */
static void append_value(seq_t **arr, size_t *cap, size_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    ++(*size);
}

/* Ensure that array K contains at least 'need' elements. */
static void ensure_k(idx_t need)
{
    while ((idx_t)sz_K < need) {
        idx_t k = next_k++;
        seq_t p1 = (seq_t)k * 6 + 1;    /* 6k+1   */
        seq_t p0 = (seq_t)k * 6 - 1;    /* 6k-1   */
        if (is_prime((idx_t)p1) && !is_prime((idx_t)p0))
            append_value(&K, &cap_K, &sz_K, k);
    }
}

/* Ensure that array M contains at least 'need' elements. */
static void ensure_m(idx_t need)
{
    while ((idx_t)sz_M < need) {
        idx_t m = next_m++;
        seq_t p0 = (seq_t)m * 6 - 1;    /* 6m-1   */
        seq_t p1 = (seq_t)m * 6 + 1;    /* 6m+1   */
        if (is_prime((idx_t)p0) && !is_prime((idx_t)p1))
            append_value(&M, &cap_M, &sz_M, m);
    }
}

/* External function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1; just return 0 */
    ensure_k(n);
    ensure_m(n);
    /* the two sequences are aligned by construction */
    return K[n - 1] - M[n - 1];
}
