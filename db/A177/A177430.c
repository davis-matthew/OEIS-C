/*
 * A177430  Least monotonically increasing logarithmic derivative consisting
 *          of only squares.
 * https://oeis.org/A177430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A177431(n)^2 (where A177431 is defined recursively
 *            below). Recurrence: a(1) = 1. For n > 1 let S_n = Σ_{i=1}^{n-1}
 *            a(i) * b(n-i) with b(1) = 1 and for k ≥ 2, b(k) = (S_k + a(k)) /
 *            k. Then a(n) is the smallest square > a(n-1) such that (S_n +
 *            a(n)) is divisible by n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 27, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *a_arr = NULL;    /* a[1] .. a[max]  (index 0 unused) */
static seq_t *b_arr = NULL;    /* b[1] .. b[max]  (index 0 unused) */
static size_t cap = 0;         /* current array capacity      */
static idx_t  computed = 0;    /* largest n computed so far   */

/* Ensure a_arr and b_arr can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cap > (size_t)n) return;            /* already big enough */

    size_t new_cap = (size_t)n + 1;         /* indices 0..n */
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Compute a(k) and b(k) for all k from computed+1 up to n */
static void compute_up_to(idx_t n)
{
    if (n <= computed) return;

    if (computed == 0) {                  /* initialise a(1)=b(1)=1 */
        ensure_capacity(1);
        a_arr[1] = 1;
        b_arr[1] = 1;
        computed = 1;
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        /* 1. Compute S_k = Σ_{i=1}^{k-1} a(i) * b(k-i) */
        seq_t S = 0;
        for (idx_t i = 1; i < k; ++i) {
            S += a_arr[i] * b_arr[k - i];
        }

        /* 2. Find the smallest integer m > sqrt(a(k-1)) with
           m^2 ≡ -S (mod k).  This ensures (S + m^2) is divisible by k. */
        seq_t prev_sqrt = floor_sqrt(a_arr[k - 1]);   /* floor(sqrt(a(k-1))) */
        seq_t m_start = prev_sqrt + 1;

        int k_int = (int)k;                      /* k fits in int for modulus ops */
        seq_t m = m_start;
        seq_t target = (seq_t)((k - (long long)(S % k)) % k);  /* -S mod k */

        /* Linear search for the first m satisfying the congruence */
        while ( ((m * m) % k_int) != target ) {
            ++m;
        }

        a_arr[k] = m * m;

        /* 3. Compute b(k) = (S + a(k)) / k */
        b_arr[k] = (S + a_arr[k]) / (seq_t)k;
    }

    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    ensure_capacity(n);
    compute_up_to(n);
    return a_arr[n];
}
