/*
 * A127021  Number of rhyme schemes (see reference for precise definition).
 * https://oeis.org/A127021
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + Bell(n-1) - 2^(n-2), a(4) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* B(m) denotes the Bell number B(m).                   */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Dynamic array helper (reallocates growing by factor 2).            */
/* ------------------------------------------------------------------ */
static void
ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap <= needed) {
        size_t new_cap = (*cap == 0) ? 1 : *cap << 1;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Prepare powers of two: pow2[i] = 2^i  for i >= 0.                  */
/* ------------------------------------------------------------------ */
static seq_t *pow2   = NULL;
static size_t pow2_cap  = 0;
static size_t pow2_len  = 0;   /* length of allocated array */

static void
ensure_pow2_up_to(size_t idx)
{
    if (pow2_len > idx) return;           /* already ok */
    if (pow2_len == 0) {                  /* allocate first element */
        ensure_cap(&pow2, &pow2_cap, 1);
        pow2[0] = (seq_t)1;
        pow2_len = 1;
    }
    for (size_t i = pow2_len; i <= idx; ++i) {
        seq_t val = pow2[i - 1] << 1;     /* 2^(i-1) << 1 = 2^i */
        ensure_cap(&pow2, &pow2_cap, i + 1);
        pow2[i] = val;
    }
    pow2_len = idx + 1;
}

/* ------------------------------------------------------------------ */
/* Bell numbers B[0..n]                                                 */
/* ------------------------------------------------------------------ */
static seq_t *bell = NULL;
static size_t bell_cap  = 0;
static size_t bell_len  = 0;   /* number of computed Bell values */

static void
compute_bell_up_to(size_t N)
{
    if (bell_len > N) return;
    if (bell_len == 0) {                  /* B[0] = 1 */
        ensure_cap(&bell, &bell_cap, 1);
        bell[0] = (seq_t)1;
        bell_len = 1;
    }
    for (size_t n = bell_len; n <= N; ++n) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;           /* C(n-1,0) */
        size_t m = n - 1;
        for (size_t k = 0; k <= m; ++k) {
            sum += binom * bell[k];
            if (k < m) {                  /* update binom to C(n-1,k+1) */
                /* binom = binom * (m-k) / (k+1) */
                seq_t num = binom * (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                binom = num / den;        /* exact division */
            }
        }
        ensure_cap(&bell, &bell_cap, n + 1);
        bell[n] = sum;
    }
    bell_len = N + 1;
}

/* ------------------------------------------------------------------ */
/* Sequence a(n) with memoisation                                       */
/* ------------------------------------------------------------------ */
static seq_t *a_arr   = NULL;
static size_t a_cap   = 0;
static size_t a_len   = 0;   /* largest index stored (>=4) */

static void
compute_a_up_to(size_t N)
{
    if (a_len > N) return;
    if (a_len == 0) {                   /* initialise a(4) = 1 */
        ensure_cap(&a_arr, &a_cap, 5);    /* indices 0..4 needed at least */
        a_arr[4] = (seq_t)1;
        a_len   = 4;
    }
    /* Pre‑compute required Bell numbers and powers of two.          */
    compute_bell_up_to(N - 1);
    ensure_pow2_up_to(N - 2);

    for (size_t n = a_len + 1; n <= N; ++n) {
        seq_t pow2_val = pow2[n - 2];    /* 2^(n-2) */
        seq_t term = ((seq_t)4) * a_arr[n - 1] + bell[n - 1] - pow2_val;
        ensure_cap(&a_arr, &a_cap, n + 1);
        a_arr[n] = term;
    }
    a_len = N;
}

/* ------------------------------------------------------------------ */
/* Public function a(idx_t n)                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined for n < 4 */
    compute_a_up_to((size_t)n);
    return a_arr[(size_t)n];
}
