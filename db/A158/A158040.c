/*
 * A158040  Determinant of power series of gamma matrix with determinant 2!.
 * https://oeis.org/A158040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = det( sum_{k=1}^{n} A^k ), where A =
 *            {{1,1,1},{1,2,1},{1,2,3}}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   det3, mat_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/det3.h"
#include "../utils/mat_add.h"

static seq_t *cache = NULL;          /* cache of a(n) values (1‑based) */
static idx_t cache_size = 0;         /* highest computed n */

/* last power A^current_n and sum S_n = sum_{k=1}^{current_n} A^k   */
static seq_t last_power[3][3];
static seq_t last_sum[3][3];
static idx_t current_n = 0;          /* n for which last_power and last_sum are valid */

/* constant matrix A */
static const seq_t A[3][3] = {
    {(seq_t)1, (seq_t)1, (seq_t)1},
    {(seq_t)1, (seq_t)2, (seq_t)1},
    {(seq_t)1, (seq_t)2, (seq_t)3}
};

/* matrix multiplication: res = a * b */
static void mat_mul(const seq_t a[3][3], const seq_t b[3][3], seq_t res[3][3])
{
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
        {
            seq_t t = a[i][0] * b[0][j];
            t += a[i][1] * b[1][j];
            t += a[i][2] * b[2][j];
            res[i][j] = t;
        }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* not defined for n < 1 */

    /* grow cache if necessary */
    if (n >= cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* if value already computed, return it */
    if (n <= current_n) return cache[n];

    /* extend computation up to n */
    for (idx_t i = current_n + 1; i <= n; ++i)
    {
        if (current_n == 0)
        {
            /* first term: A^1 and S_1 = A */
            for (idx_t r = 0; r < 3; ++r)
                for (idx_t c = 0; c < 3; ++c)
                    last_power[r][c] = A[r][c];
            for (idx_t r = 0; r < 3; ++r)
                for (idx_t c = 0; c < 3; ++c)
                    last_sum[r][c] = A[r][c];
        }
        else
        {
            /* compute next power: last_power * A */
            seq_t tmp[3][3];
            mat_mul(last_power, A, tmp);
            for (idx_t r = 0; r < 3; ++r)
                for (idx_t c = 0; c < 3; ++c)
                    last_power[r][c] = tmp[r][c];

            /* add to cumulative sum */
            mat_add(last_sum, last_power);
        }

        /* determinant of the current sum */
        cache[i] = det3(last_sum);
        current_n = i;
    }

    return cache[n];
}
