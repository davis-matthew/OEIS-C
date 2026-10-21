/*
 * A097642  Numbers k such that k = (d_1! + d_2! + ... + d_k!) - (d_1 + d_2 +
 *          ... + d_k) where d_1 d_2 ... d_k is the decimal expansion of k.
 * https://oeis.org/A097642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nice, nonn
 * Formula:   Find numbers k such that ** k = sum_{digits d of k} (d! - d) **
 *            The sequence is the increasing list of those k. ** Since d! - d
 *            is at most 362871 (for d=9), ** any solution must satisfy k <=
 *            7*362871 = 2540097, ** so there are only finitely many solutions
 *            and they all lie ** in the range 1..2540097
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for dynamic allocation */
#include <stdlib.h>

/* Assume seq_t and idx_t are defined elsewhere by the build system. */

static const seq_t fact_minus_digit[10] = {
    /* d! - d */
    1, 0, 0, 3, 20, 115, 714, 5033, 40312, 362871
};

static seq_t *solutions = NULL;   /* array of found k values */
static size_t sol_len   = 0;       /* number of elements found */
static size_t sol_cap   = 0;       /* capacity of the array */

/* Helper to grow the solutions array when needed. */
static void grow_solutions(void)
{
    size_t new_cap = sol_cap ? sol_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failed, but keep old array */
    solutions = tmp;
    sol_cap = new_cap;
}

/* Build all solutions once. */
static void build_solutions(void)
{
    const idx_t MAX_K = 7 * 362871U;   /* 2540097 */
    for (idx_t k = 1; k <= MAX_K; ++k) {
        idx_t tmp = k;
        seq_t sum = 0;
        while (tmp) {
            sum += fact_minus_digit[ tmp % 10 ];
            tmp /= 10;
        }
        if ((idx_t)sum == k) {
            if (sol_len == sol_cap) grow_solutions();
            solutions[sol_len++] = k;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Ensure the solution list is built once. */
    if (!solutions) build_solutions();

    if (n == 0) return 0; /* safety: n is >=1 per specification */

    if ((idx_t)n - 1 < sol_len) {
        return solutions[(idx_t)n - 1];
    } else {
        /* The request is beyond known terms; return 0 (undefined). */
        return 0;
    }
}
