/*
 * A072388  a(0) = 1; for n > 0, a(n) = floor((prime(n+1) + prime(n) +
 *          a(n-1))/3).
 * https://oeis.org/A072388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A072388
   Formula: a(0) = 1; for n>0,
   a(n) = floor((prime(n+1) + prime(n) + a(n-1)) / 3)  */

/* ------------------------------------------------------------------------- */
/*  Included headers (C11/C++17 compatible)                                 */
/* ------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*  Static storage for primes and sequence values                            */
/* ------------------------------------------------------------------------- */
#define PRIMES_INIT_CAP   128
#define SEQ_INIT_CAP     128

static uint64_t *primes = NULL;        /* primes[0] = 2, primes[1] = 3, ...           */
static size_t   primes_cnt = 0;        /* number of primes stored                   */
static uint64_t last_checked = 1;      /* next candidate to test for primality      */

static seq_t   *seq      = NULL;       /* a(0) ... a(max_computed)                  */
static size_t   seq_cap  = 0;          /* allocated capacity                        */
static size_t   seq_max  = 0;          /* largest n computed so far                 */

/* ------------------------------------------------------------------------- */
/*  Helper: grow a dynamic array of uint64_t                                   */
/* ------------------------------------------------------------------------- */
static void grow_uint64(uint64_t **arr, size_t *cnt, size_t new_len, size_t *cap)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap > 0) ? *cap : PRIMES_INIT_CAP;
        while (new_cap < new_len) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) return;      /* allocation fail: ignore, caller will handle */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------------- */
/*  Helper: ensure that at least `need` primes are stored                      */
/*  Simple incremental trial‑division generation (sufficient for n<=~10000)  */
/* ------------------------------------------------------------------------- */
static void ensure_primes(size_t need)
{
    if (need <= primes_cnt) return;

    size_t cap = (primes_cnt > 0) ? primes_cnt : PRIMES_INIT_CAP;
    if (!primes) primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cur_cap = primes_cnt > 0 ? cap : PRIMES_INIT_CAP;

    /* grow as needed */
    while (primes_cnt < need) {
        /* test candidate for primality */
        uint64_t cand = last_checked + 1;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        last_checked = cand;

        if (is_prime) {
            grow_uint64(&primes, &primes_cnt, primes_cnt + 1, &cur_cap);
            primes[primes_cnt++] = cand;
        }
    }
}

/* ------------------------------------------------------------------------- */
/*  Helper: grow a dynamic array of seq_t                                       */
/* ------------------------------------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cnt, size_t new_len, size_t *cap)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap > 0) ? *cap : SEQ_INIT_CAP;
        while (new_cap < new_len) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) return;
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------------- */
/*  Ensure that a(n) is computed for all n <= N                               */
/* ------------------------------------------------------------------------- */
static void ensure_seq(size_t N)
{
    if (N <= seq_max) return;

    /* allocate or grow sequence array */
    if (!seq) {
        seq_cap = N + 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;      /* a(0) */
        seq_max = 0;
    } else if (seq_cap <= N) {
        grow_seq(&seq, &seq_max, N + 1, &seq_cap);
    }

    /* compute terms from seq_max+1 up to N */
    for (size_t i = seq_max + 1; i <= N; ++i) {
        ensure_primes(i + 1);   /* need prime(i) => primes[i-1], prime(i+1) => primes[i] */
        seq_t sum = (seq_t)primes[i-1] + (seq_t)primes[i] + seq[i-1];
        seq[i] = sum / (seq_t)3;    /* floor division */
    }
    seq_max = N;
}

/* ------------------------------------------------------------------------- */
/*  Public function: sequence value at index n                                */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_seq((size_t)n);
    return seq[(size_t)n];
}
