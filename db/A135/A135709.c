/*
 * A135709  Not the sum of three distinct nonzero Fibonacci numbers.
 * https://oeis.org/A135709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135709: positive integers NOT representable as the sum of three
   distinct nonzero Fibonacci numbers.
   This implementation constructs a sieve of all sums of three distinct
   Fibonacci numbers up to a requested limit and then returns the
   complement. No closed‑form formula is known. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -----------------  static auxiliary state  ----------------------- */

static idx_t       max_limit   = 0;           /* highest limit computed so far */
static unsigned char *reach       = NULL;      /* reach[i] == 1 iff i is a sum */
static seq_t       *nonrep       = NULL;      /* sorted list of non‑representable ints */
static size_t     nonrep_count = 0;          /* number of elements in nonrep */
static size_t     nonrep_cap   = 0;          /* allocated capacity of nonrep */

/* ------------------------------------------------------------------ */
/* Build sieve and complementary list for all numbers up to new_limit */
/* ------------------------------------------------------------------ */
static void compute_up_to(idx_t new_limit)
{
    /* Allocate reach array and zero it */
    unsigned char *new_reach = (unsigned char *)
        calloc((size_t)new_limit + 1, sizeof(unsigned char));
    if (!new_reach) abort();
    if (reach) free(reach);
    reach = new_reach;

    /* Build the list of Fibonacci numbers 1,2,3,5,8,… ≤ new_limit */
    idx_t *fibs = NULL;
    size_t fib_cnt = 0, fib_cap = 0;
    /* First Fibonacci number */
    if ((idx_t)1 <= new_limit) {
        fibs = (idx_t *)malloc(8 * sizeof(idx_t));
        fibs[0] = 1;
        fib_cnt = 1;
        fib_cap = 8;
    }
    idx_t f_prev = 1, f_curr = 2;
    while (f_curr <= new_limit) {
        if (fib_cnt == fib_cap) {
            size_t new_cap = fib_cap ? fib_cap * 2 : 8;
            idx_t *tmp = (idx_t *)realloc(fibs, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            fibs   = tmp;
            fib_cap = new_cap;
        }
        fibs[fib_cnt++] = f_curr;
        /* compute next Fibonacci, guarding against overflow */
        if (f_prev > (UINT64_MAX - f_curr) / 1)   /* unsigned overflow check */
            break;
        idx_t f_next = f_curr + f_prev;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Mark all sums of three distinct Fibonacci numbers ≤ new_limit */
    for (size_t i = 0; i < fib_cnt; ++i) {
        for (size_t j = i + 1; j < fib_cnt; ++j) {
            for (size_t k = j + 1; k < fib_cnt; ++k) {
                seq_t sum = (seq_t)fibs[i] + (seq_t)fibs[j] + (seq_t)fibs[k];
                if (sum <= (seq_t)new_limit)
                    reach[(size_t)sum] = 1;
            }
        }
    }

    /* Rebuild the complement list (non‑representable numbers) */
    if (nonrep) free(nonrep);
    nonrep = NULL;
    nonrep_count = 0;
    nonrep_cap   = 0;

    for (idx_t num = 1; num <= new_limit; ++num) {
        if (!reach[(size_t)num]) {
            if (nonrep_count == nonrep_cap) {
                size_t new_cap = nonrep_cap ? nonrep_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(nonrep, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                nonrep = tmp;
                nonrep_cap = new_cap;
            }
            nonrep[nonrep_count++] = (seq_t)num;
        }
    }

    max_limit = new_limit;
    free(fibs);
}

/* ------------------------------------------------------------------ */
/* Ensure that the non‑representable list contains at least *n* terms */
/* ------------------------------------------------------------------ */
static void ensure_n(size_t n)
{
    if (nonrep_count >= n) return;
    idx_t new_limit = max_limit ? max_limit * 2 : (idx_t)100000;
    while (true) {
        compute_up_to(new_limit);
        if (nonrep_count >= n) break;
        new_limit *= 2;
    }
}

/* ---------------------------  public function  --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; undefined for n=0 */
    size_t idx = (size_t)n;
    ensure_n(idx);
    return nonrep[idx - 1];
}
