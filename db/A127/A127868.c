/*
 * A127868  Number of square tiles in all tilings of a 3 X n board with 1 X 1
 *          and L-shaped tiles (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A127868 – Number of square tiles in all tilings of a 3×n board
 * with 1×1 and L‑shaped tiles (covered squares = 3).  
 *
 * Generating function (OEIS):  
 *   3x(1-x)^2(1+6x+3x^2) / (1-3x-7x^2+x^3-2x^4)^2
 *
 * Recurrence (valid for n ≥ 9):
 *   a(n) = 6 a(n‑1) + 5 a(n‑2) – 44 a(n‑3) – 39 a(n‑4)
 *           + 2 a(n‑5) – 29 a(n‑6) + 4 a(n‑7) – 4 a(n‑8)
 *
 * Initial values:
 *   a(1)=3, a(2)=30, a(3)=171, a(4)=1044,
 *   a(5)=5691, a(6)=30678, a(7)=159891, a(8)=821100.
 *
 * The function below implements this recurrence iteratively; no dynamic
 * allocation or memoisation is required, so it compiles in both C11 and
 * C++17 for any seq_t type.
 */

#include <stddef.h>

static const seq_t base[8] = {
    3,        /* a(1) */
    30,       /* a(2) */
    171,      /* a(3) */
    1044,     /* a(4) */
    5691,     /* a(5) */
    30678,    /* a(6) */
    159891,   /* a(7) */
    821100    /* a(8) */
};

seq_t a(idx_t n)
{
    /* For n == 0 the function returns 0; OEIS offset is 1, so this
       value is never used in standard queries. */
    if (n == 0) return 0;

    if (n <= 8) return base[n - 1];

    /* ring buffer holding the last 8 values: last[0]=a(n-8), …, last[7]=a(n-1) */
    seq_t last[8];
    for (size_t i = 0; i < 8; ++i) last[i] = base[i];

    for (idx_t cur = 9; cur <= n; ++cur) {
        /* compute a(cur) using the recurrence */
        seq_t val = 6 * last[7]      /* a(n-1) */
                  + 5 * last[6]      /* a(n-2) */
                  - 44 * last[5]     /* a(n-3) */
                  - 39 * last[4]     /* a(n-4) */
                  + 2 * last[3]      /* a(n-5) */
                  - 29 * last[2]     /* a(n-6) */
                  + 4 * last[1]      /* a(n-7) */
                  - 4 * last[0];     /* a(n-8) */

        /* shift buffer left by one: discard a(n-8) and append val as a(n) */
        for (size_t j = 0; j < 7; ++j) last[j] = last[j + 1];
        last[7] = val;
    }

    return last[7];   /* now last[7] holds a(n) */
}
