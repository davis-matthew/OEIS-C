/*
 * A205523  Numbers k such that gcd(k, sigma(k)) == sigma(k) (mod k).
 * https://oeis.org/A205523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that gcd(k, σ(k)) ≥ σ(k) mod k.
   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 6, a(6) = 7, a(7) = 11, ... */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_LIMIT 200000          /* enough to cover the first 10 000 terms */

static seq_t *sigma_arr = NULL;   /* σ(k) for 0 ≤ k ≤ MAX_LIMIT */
static seq_t *seq_list = NULL;    /* list of sequence terms */
static size_t seq_len  = 0;       /* number of terms stored */
static bool  initialized = false;

/* Build σ(k) for 1 ≤ k ≤ MAX_LIMIT and collect the sequence. */
static void compute_sequence(void)
{
    size_t i, j;

    /* Allocate and initialise σ array */
    sigma_arr = (seq_t *)malloc((MAX_LIMIT + 1) * sizeof(seq_t));
    for (i = 0; i <= MAX_LIMIT; ++i) sigma_arr[i] = 0;

    /* Classic divisor‑sum sieve */
    for (i = 1; i <= MAX_LIMIT; ++i)
    {
        for (j = i; j <= MAX_LIMIT; j += i)
            sigma_arr[j] += (seq_t)i;
    }

    /* Dynamic allocation of the sequence list */
    seq_list = NULL;
    seq_len  = 0;
    size_t cap = 0;

    for (i = 1; i <= MAX_LIMIT; ++i)
    {
        seq_t s = sigma_arr[i];
        seq_t g = gcd((seq_t)i, s);
        seq_t m = s % (seq_t)i;          /* remainder of σ(i) upon division by i */

        if (g >= m)                       /* condition gcd ≥ σ mod i */
        {
            if (seq_len == cap)
            {
                cap = (cap == 0) ? 1024 : cap << 1;
                seq_list = (seq_t *)realloc(seq_list, cap * sizeof(seq_t));
            }
            seq_list[seq_len++] = (seq_t)i;
        }
    }

    free(sigma_arr);
    sigma_arr = NULL;
    initialized = true;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (!initialized)
        compute_sequence();

    /* All indices are 1‑based. */
    if ((size_t)n == 0 || (size_t)n > seq_len)
        return (seq_t)0;                  /* out of range */

    return seq_list[n - 1];
}
