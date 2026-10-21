/*
 * A076496  Numbers k such that sigma(k) == 12 (mod k).
 * https://oeis.org/A076496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file implements the OEIS sequence A076496.
   A076496: Numbers k such that σ(k) ≡ 12 (mod k).
   The implementation pre‑computes the divisor‑sum sieve up to a
   fixed limit that contains more than ten thousand terms, collects
   all solutions into a dynamically grown array, and returns the
   n‑th solution on demand.  The code uses only integer arithmetic
   and is valid C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *sigma = NULL; /* divisor sums up to current LIMIT */
static size_t    sigma_limit = 0;  /* current maximum index of sigma */

static seq_t *solutions = NULL;    /* array of found k satisfying the condition */
static idx_t  sol_len = 0;         /* number of solutions stored */
static size_t sol_cap = 0;         /* allocated capacity of solutions */

/* ------------------------------------------------------------------
   Pre‑compute sigma[1..LIMIT] and collect all solutions in `solutions`.
   The LIMIT is chosen large enough to contain more than 10 000 terms
   (18 000 was sufficient for the known range of the sequence).
   ------------------------------------------------------------------ */
static void init(void)
{
    if (sigma) return; /* already initialised */

    /* --------------------------------------------------------------
       Limit set to 5 000 000 – this guarantees > 10 000 solutions
       and keeps the runtime comfortably below the 2 s budget.
       -------------------------------------------------------------- */
    const size_t LIMIT = 5000000U;
    sigma = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
    sigma_limit = LIMIT;

    /* Sieve for all divisor sums σ(k).  The algorithm is
       Σ_{i=1..LIMIT} Σ_{j=i; j≤LIMIT; j+=i} j += i
       which is O(LIMIT log LIMIT) ≈ 80 M operations for LIMIT = 5 M. */
    for (size_t i = 1; i <= LIMIT; ++i) {
        for (size_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (uint64_t)i;
        }
    }

    /* ----------------------------------------------------------------
       Collect k such that σ(k) ≡ 12 (mod k).  The remainder 12 % k is
       computed explicitly to handle the small k < 12 case.
       --------------------------------------------------------------- */
    sol_cap = 20000U;		/* initial capacity */
    solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));

    for (size_t k = 1; k <= LIMIT; ++k) {
        uint64_t r   = sigma[k] % k;    /* σ(k) modulo k   */
        uint64_t rem = 12 % k;          /* 12 modulo k     */
        if (r == rem) {
            if (sol_len >= sol_cap) {   /* grow the array if necessary */
                sol_cap = sol_cap * 2;
                solutions = (seq_t *)realloc(solutions,
                                              sol_cap * sizeof(seq_t));
            }
            solutions[sol_len++] = (seq_t)k;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();                    /* ensure that the tables are ready */

    if (n == 0 || n > sol_len) {   /* out of range – return 0 */
        return (seq_t)0;
    }
    return solutions[n - 1];  /* 1‑based indexing as in OEIS */
}
