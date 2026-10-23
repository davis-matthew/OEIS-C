/*
 * A107252  a(n) = Product_{k=0..n-1} (n+k)!/(k+1)!.
 * https://oeis.org/A107252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1} (n+k)!/(k+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for factorials and the sequence */
static seq_t *fact = NULL;          /* fact[i] = i!           */
static idx_t fact_last = 0;         /* highest computed index */
static idx_t fact_cap = 0;          /* current capacity     */

static seq_t *a_arr = NULL;        /* a_arr[i] = a(i)       */
static idx_t a_last = 0;           /* highest computed index */
static idx_t a_cap = 0;            /* current capacity      */

/* Ensure factorial array can provide values up to idx 'need' */
static void ensure_fact(idx_t need)
{
    if (need <= fact_last) return;

    if (fact_cap <= need) {
        size_t new_cap = (fact_cap == 0) ? 1 : fact_cap;
        while (new_cap <= need) new_cap <<= 1;
        fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }
    for (idx_t i = fact_last + 1; i <= need; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_last = need;
}

/* Ensure sequence table holds values up to idx 'need' */
static void ensure_seq(idx_t need)
{
    if (need <= a_last) return;

    if (a_cap <= need) {
        size_t new_cap = (a_cap == 0) ? 1 : a_cap;
        while (new_cap <= need) new_cap <<= 1;
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    for (idx_t i = a_last + 1; i <= need; ++i) {
        /* r1 = fact[2i-2] / fact[i-1]  (product i .. 2i-2) */
        seq_t r1 = fact[2 * i - 2] / fact[i - 1];
        /* r2 = fact[2i-1] / fact[i]    (product i+1 .. 2i-1) */
        seq_t r2 = fact[2 * i - 1] / fact[i];
        /* factor = r1 * r2 */
        seq_t factor = r1 * r2;
        a_arr[i] = a_arr[i - 1] * factor;
    }
    a_last = need;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Lazy one‑time init of index 0 entry */
    if (a_cap == 0) {
        a_cap = 1;
        a_arr = (seq_t*)malloc(sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        a_last = 0;

        fact_cap = 1;
        fact = (seq_t*)malloc(sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact_last = 0;
    }

    if (n > a_last) {
        ensure_fact(2 * n);          /* need factorials up to 2n   */
        ensure_seq(n);
    }

    return a_arr[n];
}
