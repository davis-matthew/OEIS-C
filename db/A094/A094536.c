/*
 * A094536  Number of binary words of length n that are not "bifix-free".
 * https://oeis.org/A094536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094536: Number of binary words of length n that are not bifix‑free
 *
 * Recurrence (from OEIS):
 *   a(0) = 0, a(1) = 0
 *   a(2*n+1) = 2*a(2*n)
 *   a(2*n)   = 2*a(2*n-1) + 2^n - a(n)
 *
 * The function keeps a memoized table of already computed values up to the
 * largest n requested, so subsequent calls are O(1) for already known terms
 * and O(n) to extend the table.  The implementation uses only integer
 * arithmetic; no floating point or external utilities are required.
 */

#include <stdlib.h>

/* memoization tables */
static seq_t *A = NULL;           /* a[n] */
static size_t A_len = 0, A_cap = 0;
static seq_t *Pow2 = NULL;        /* pow2[i] = 2^i */
static size_t Pow2_len = 0, Pow2_cap = 0;

/* expand an array to hold at least 'needed' more elements (including current) */
static void grow_seq(seq_t **arr, size_t *len, size_t *cap, size_t needed)
{
    if (*cap < *len + needed) {
        size_t new_cap = (*cap < 4 ? 4 : *cap) * 2;
        while (new_cap < *len + needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ensure pow2[0..needed] are computed */
static void ensure_pow2(size_t needed)
{
    if (needed + 1 > Pow2_cap) {
        grow_seq(&Pow2, &Pow2_len, &Pow2_cap, needed + 1 - Pow2_len);
    }
    if (Pow2_len == 0) {
        Pow2[0] = (seq_t)1;
        Pow2_len = 1;
    }
    for (size_t i = Pow2_len; i <= needed; ++i) {
        Pow2[i] = Pow2[i - 1] * (seq_t)2;
    }
    Pow2_len = needed + 1;
}

/* ensure a[0..needed] are computed */
static void ensure_a(size_t needed)
{
    if (needed + 1 > A_cap) {
        grow_seq(&A, &A_len, &A_cap, needed + 1 - A_len);
    }
    if (A_len == 0) {
        A[0] = (seq_t)0;
        A_len = 1;
    }
    if (A_len == 1) {
        A[1] = (seq_t)0;
        A_len = 2;
    }
    for (size_t i = A_len; i <= needed; ++i) {
        if (i == 0) { A[i] = (seq_t)0; continue; }
        if (i == 1) { A[i] = (seq_t)0; continue; }
        if ((i & 1) == 0) {             /* even i = 2*n */
            size_t n = i >> 1;
            A[i] = A[i - 1] * (seq_t)2 + Pow2[n] - A[n];
        } else {                         /* odd i = 2*n+1 */
            A[i] = A[i - 1] * (seq_t)2;
        }
    }
    A_len = needed + 1;
}

seq_t a(idx_t n)
{
    size_t up = (size_t)n;
    if (up < 2) {
        /* a(0)=0, a(1)=0 are already handled during first call */
        ensure_a(up);
        return A[up];
    }
    /* ensure pow2 up to floor(up/2) and a up to up */
    ensure_pow2(up >> 1);
    ensure_a(up);
    return A[up];
}
