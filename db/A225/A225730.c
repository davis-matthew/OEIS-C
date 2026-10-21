/*
 * A225730  Numbers k such that k < d(k)^(22/10), where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A225730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers k such that k^5 < d(k)^11, where d(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 63, u16 n <= 155, u32 n <= 155, u64 n <= 155,
 *            u128 n <= 155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   - We pre‑compute all qualifying k up to a fixed upper bound
     (chosen large enough that no further solutions exist).
   - The inequality is tested using 128‑bit arithmetic to avoid
     overflow for all values that fit in seq_t.
   - The result array is built once on the first call; subsequent
     calls use the cached list.
   - The function is valid C11 and C++17 and respects all type
     constraints above. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *solutions = NULL;          /* cached solutions */
static size_t sol_len = 0;                /* number of solutions stored */
static size_t sol_cap = 0;                /* current capacity of solutions */
static bool initialized = false;         /* whether init() has run */

static void init(void)
{
    /* Upper bound large enough to contain all solutions. */
    const unsigned int MAX_K = 500000u;   /* 5×10^5 is safe for this sequence   */

    /* Allocate array for divisor counts (using uint16_t, which is sufficient). */
    uint16_t *divcnt = (uint16_t *)calloc((size_t)MAX_K + 1, sizeof(uint16_t));
    if (!divcnt) return;  /* allocation failed – keep initialized false */

    /* Simple divisor count sieve: O(n log n). */
    for (unsigned int i = 1; i <= MAX_K; ++i) {
        for (unsigned int j = i; j <= MAX_K; j += i)
            ++divcnt[j];
    }

    /* Allocate initial capacity for solutions. */
    sol_cap = 64;
    solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
    if (!solutions) {
        free(divcnt);
        return;  /* allocation failed – keep initialized false */
    }
    sol_len = 0;

    /* Find all k satisfying k^5 < d(k)^11. */
    for (unsigned int k = 2; k <= MAX_K; ++k) {
        unsigned int d = divcnt[k];

        /* Compute k^5 and d^11 using 128‑bit arithmetic. */
        unsigned __int128 left  = 1;
        for (int i = 0; i < 5; ++i)
            left *= (unsigned __int128)k;

        unsigned __int128 right = 1;
        for (int i = 0; i < 11; ++i)
            right *= (unsigned __int128)d;

        if (left < right) {
            /* Grow solutions array if necessary. */
            if (sol_len == sol_cap) {
                sol_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
                if (!tmp) {
                    free(divcnt);
                    free(solutions);
                    solutions = NULL;
                    sol_len = sol_cap = 0;
                    return;  /* allocation failed – keep initialized false */
                }
                solutions = tmp;
            }
            solutions[sol_len++] = (seq_t)k;
        }
    }

    free(divcnt);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (!initialized || n < 1 || (size_t)n > sol_len)
        return (seq_t)0;          /* undefined for out‑of‑range indices */

    return solutions[(size_t)n - 1];
}
