/*
 * A177029  Numbers that have exactly two different representations as
 *          polygonal numbers.
 * https://oeis.org/A177029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that have exactly two different representations as polygonal numbers.
   The function builds the sequence by enumerating all polygonal numbers up to a
   dynamically increasing limit and collects those with exactly two representations. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* local integer square root for unsigned 64‑bit integers */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* static state for the sequence */
static uint8_t *freq = NULL;          /* counts of representations */
static idx_t  freq_len = 0;           /* size of freq array   */
static seq_t *seq  = NULL;            /* resulting sequence   */
static size_t seq_len = 0;            /* number of terms   */
static size_t seq_cap = 0;            /* allocated capacity for seq */
static idx_t  current_L = 0;          /* current upper bound touched */

/* recompute the sequence up to upper bound new_L */
static void recompute(idx_t new_L)
{
    /* free old frequency table */
    if (freq != NULL) {
        free(freq);
    }
    freq_len = new_L + 1;
    freq = (uint8_t *)calloc(freq_len, sizeof(uint8_t));
    if (freq == NULL) exit(1);

    /* 1. contributions where m = 2  =>  value = k   (k ≥ 3) */
    for (idx_t v = 3; v <= new_L; ++v) {
        ++freq[v];
    }

    /* 2. contributions where m ≥ 3 */
    idx_t Mmax = int_sqrt_u64(2 * new_L);
    for (idx_t m = 3; m <= Mmax; ++m) {
        idx_t denom = m * (m - 1);                     /* m*(m‑1) > 0 */
        idx_t numer = 2 * new_L + 2 * m * m - 4 * m;    /* 2L + 2m² – 4m  */
        idx_t k_max = numer / denom;
        if (k_max < 3) continue;
        if (k_max > new_L) k_max = new_L;

        for (idx_t k = 3; k <= k_max; ++k) {
            int64_t term = (int64_t)(k - 2) * m * m
                          - (int64_t)(k - 4) * m;
            idx_t val = (idx_t)(term / 2);
            if (val > new_L) break;
            ++freq[val];
        }
    }

    /* rebuild the sequence array (numbers with exactly two representations) */
    if (seq != NULL) {
        free(seq);
    }
    seq_cap = new_L;                     /* worst‑case upper bound */
    seq_len = 0;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (seq == NULL) exit(1);

    for (idx_t v = 1; v <= new_L; ++v) {
        if (freq[v] == 2) {
            seq[seq_len++] = (seq_t)v;
        }
    }

    current_L = new_L;
}

/* make sure the sequence contains at least n terms */
static void ensure_seq(idx_t n)
{
    if (seq_len >= n) return;

    /* start with a bound that surely covers the needed number */
    idx_t target_L = current_L * 2 + 10;
    if (target_L < 100) target_L = 100;          /* small start */
    while (seq_len < n) {
        if (target_L < n * 10) target_L = n * 10;
        recompute(target_L);
        if (seq_len >= n) break;
        target_L *= 2;                           /* increase bound */
    }
}

/* exported function required by the system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                        /* safety for out of range */
    ensure_seq(n);
    return seq[n - 1];
}
