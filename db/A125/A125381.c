/*
 * A125381  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A125381: Number of base-18 circular n‑digit numbers with
   adjacent digits differing by ≤5.
   The sequence equals the trace of the n‑th power of the 18×18
   adjacency matrix A, where A[i][j] = 1 if |i-j| ≤ 5 else 0.
   a(0) = 1.
*/
#include <stdbool.h>
#include <stdint.h>

static void mat_mul(const seq_t A[18][18], const seq_t B[18][18], seq_t R[18][18])
{
    for (idx_t i = 0; i < 18; ++i) {
        for (idx_t j = 0; j < 18; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 18; ++k)
                sum = sum + (A[i][k] * B[k][j]);
            R[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* static adjacency matrix, init once */
    static seq_t adj[18][18];
    static bool init = false;
    if (!init) {
        for (idx_t i = 0; i < 18; ++i)
            for (idx_t j = 0; j < 18; ++j) {
                idx_t diff = (i > j) ? (i - j) : (j - i);
                adj[i][j] = (diff <= 5) ? (seq_t)1 : 0;
            }
        init = true;
    }

    /* matrices for exponentiation */
    seq_t result[18][18]; /* identity */
    seq_t base[18][18];   /* current power of adj */
    seq_t temp[18][18];

    for (idx_t i = 0; i < 18; ++i)
        for (idx_t j = 0; j < 18; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : 0;
            base[i][j]   = adj[i][j];
        }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(result, base, temp);
            for (idx_t i = 0; i < 18; ++i)
                for (idx_t j = 0; j < 18; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(base, base, temp);
            for (idx_t i = 0; i < 18; ++i)
                for (idx_t j = 0; j < 18; ++j)
                    base[i][j] = temp[i][j];
        }
    }

    /* trace of result */
    seq_t ans = 0;
    for (idx_t i = 0; i < 18; ++i)
        ans = ans + result[i][i];
    return ans;
}
