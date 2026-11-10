/*
 * A224799  Column 0 of the matrix square of triangle A224798.
 * https://oeis.org/A224799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k)*T(k,0), where T = A224798 with
 *            T(i,i)=1, T(i,i-1)=2^(i-1), T(i,j)=2*T(i,j+1)+Sum_{k=j+2..i-1}
 *            T(i,k)*T(k,j+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-07)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t **A224799_T = NULL;
static size_t A224799_rows = 0;

static void A224799_ensure(size_t needed)
{
    if (A224799_rows > needed) return;
    A224799_T = (seq_t**)realloc(A224799_T, (needed + 1) * sizeof(seq_t*));
    for (size_t i = A224799_rows; i <= needed; ++i) {
        A224799_T[i] = mem_new(seq_t, (i + 1));
        if (i == 0) {
            A224799_T[0][0] = (seq_t)1;
            continue;
        }
        A224799_T[i][i] = (seq_t)1;
        A224799_T[i][i-1] = pow2((idx_t)(i - 1));
        for (size_t jj = i - 1; jj-- > 0; ) {
            seq_t sum = 0;
            for (size_t k = jj + 2; k < i; ++k)
                sum += A224799_T[i][k] * A224799_T[k][jj+1];
            A224799_T[i][jj] = (seq_t)2 * A224799_T[i][jj+1] + sum;
        }
    }
    A224799_rows = needed + 1;
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    A224799_ensure(m);
    seq_t s = 0;
    for (size_t k = 0; k <= m; ++k)
        s += A224799_T[m][k] * A224799_T[k][0];
    return s;
}
