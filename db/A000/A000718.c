/*
 * A000718  Boustrophedon transform of triangular numbers 1,1,3,6,10,...
 * https://oeis.org/A000718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon (Seidel) transform of b(0)=1, b(k)=k*(k+1)/2:
 *            T(0,0)=1; row k starts with b(k) and accumulates the previous
 *            row in alternating direction; a(n) = last entry of row n.
 *            E.g.f.: (sec(x) + tan(x))*(exp(x)*(x + 1/2*x^2) + 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A718_MAX 20002

static seq_t a718_row[A718_MAX];
static seq_t a718_res[A718_MAX];
static idx_t a718_done = 0;

seq_t a(idx_t n) {
    if (n >= (idx_t)(A718_MAX - 1)) return 0;
    if (a718_done == 0) {
        a718_row[0] = 1;
        a718_res[0] = 1;
        a718_done = 1;
    }
    while (a718_done <= n) {
        idx_t i = a718_done;
        seq_t ii = (seq_t)i;
        seq_t b = ii * (ii + 1) / 2;
        if (i & 1) {
            /* right to left: T(i,j) = T(i,j+1) + T(i-1,j) */
            a718_row[i] = b;
            for (idx_t j = i; j-- > 0;)
                a718_row[j] = a718_row[j] + a718_row[j + 1];
            a718_res[i] = a718_row[0];
        } else {
            /* left to right: T(i,j) = T(i,j-1) + T(i-1,j-1) */
            seq_t prev = a718_row[0];
            a718_row[0] = b;
            for (idx_t j = 1; j <= i; j++) {
                seq_t tmp = 0;
                if (j < i) tmp = a718_row[j];
                a718_row[j] = a718_row[j - 1] + prev;
                prev = tmp;
            }
            a718_res[i] = a718_row[i];
        }
        a718_done++;
    }
    return a718_res[n];
}
