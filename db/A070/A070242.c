/*
 * A070242  a(n) = Card( k>0 : sigma(k)=sigma(n) ).
 * https://oeis.org/A070242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # { k > 0 : σ(k) = σ(n) } (A070242)
 * Range:     u8 none, u16 n <= 26, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;          /* indicates whether data was built */
static idx_t   sigma_limit;               /* maximum σ value considered (≈σ(10000)) */
static seq_t *sigma_arr = NULL;           /* σ(k) for k = 0 … sigma_limit */
static seq_t *freq_arr  = NULL;           /* freq_arr[s] = # { k : σ(k) = s } for s ≤ sigma_limit */

/* Build the tables the first time a(n) is requested. */
static void init_tables(void)
{
    const idx_t N0 = 10000;              /* range for preliminary σ(max) search */

    /* ---- 1. compute σ for 1 … N0 to find σ(max) ---------------- */
    seq_t *sigma_temp = (seq_t*)malloc((N0 + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= N0; ++i) sigma_temp[i] = 0;

    for (idx_t d = 1; d <= N0; ++d) {
        for (idx_t j = d; j <= N0; j += d) {
            sigma_temp[j] += (seq_t)d;
        }
    }

    seq_t sigma_max = 0;
    for (idx_t i = 1; i <= N0; ++i) {
        if (sigma_temp[i] > sigma_max) sigma_max = sigma_temp[i];
    }

    /* ---- 2. determine the limit for the full table -------------- */
    sigma_limit = (idx_t)sigma_max;
    if (sigma_limit < N0) sigma_limit = N0;   /* safety guard */

    /* ---- 3. allocate the full tables --------------------------- */
    sigma_arr = (seq_t*)malloc((sigma_limit + 1) * sizeof(seq_t));
    freq_arr  = (seq_t*)malloc((sigma_limit + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= sigma_limit; ++i) {
        sigma_arr[i] = 0;
        freq_arr[i]  = 0;
    }

    /* ---- 4. compute σ for 1 … sigma_limit ---------------------- */
    for (idx_t d = 1; d <= sigma_limit; ++d) {
        for (idx_t j = d; j <= sigma_limit; j += d) {
            sigma_arr[j] += (seq_t)d;
        }
    }

    /* ---- 5. build frequency table ------------------------------ */
    for (idx_t k = 1; k <= sigma_limit; ++k) {
        seq_t s = sigma_arr[k];
        if (s <= sigma_limit) {                /* count only values that occur in the range of interest */
            freq_arr[(idx_t)s] += (seq_t)1;
        }
    }

    free(sigma_temp);
    initialized = true;
}

/* ----------------------------------------------------------------
 *  Return the n-th term of A070242
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    if (n == 0) return 0;                 /* the sequence is 1‑indexed; 0 is unused */

    /* σ(n) is guaranteed to be ≤ sigma_limit because n ≤ 10000 */
    seq_t sigma_n = sigma_arr[n];
    idx_t s_index = (idx_t)sigma_n;       /* cast σ(n) to index type */
    return freq_arr[s_index];
}
