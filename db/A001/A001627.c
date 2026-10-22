/*
 * A001627  Related to Latin rectangles.
 * https://oeis.org/A001627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = A000186(n) + A000186(n-1) +
 *            3*(n-1)*A001626(n-1), where A000186(n) = Sum_{k=0..floor(n/2)}
 *            binomial(n,k)*D(n-k)*D(k)*U(n-2k) (D = derangements A000166:
 *            D(0)=1, D(1)=0, D(n)=(n-1)*(D(n-1)+D(n-2)); U = menage numbers
 *            A000179 with U(0)=1, U(1)=-1, U(2)=0, U(3)=1, U(4)=2, U(n) =
 *            n*U(...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a001028_z.h"

static seq_t *a1627_K = NULL, *a1627_B = NULL;
static seq_t *a1627_D = NULL, *a1627_U = NULL, *a1627_row = NULL;
static idx_t a1627_cap = 0;

static void a1627_build(idx_t N)
{
    idx_t i, m, k;
    seq_t *D = a001028_z(N + 6);
    seq_t *U = a001028_z(N + 6);
    seq_t *row = a001028_z(N + 6);
    seq_t *K = a001028_z(N + 6);
    seq_t *B = a001028_z(N + 6);

    D[0] = 1;
    D[1] = 0;
    for (i = 2; i <= N; i++)
        D[i] = (seq_t)(i - 1) * (D[i - 1] + D[i - 2]);

    U[0] = 1;
    U[1] = 0; /* actual value -1, handled separately */
    U[2] = 0;
    U[3] = 1;
    U[4] = 2;
    U[5] = 13;
    for (i = 6; i <= N; i++)
        U[i] = ((seq_t)i * U[i - 1] + U[i - 2] * 2)
             - ((seq_t)(i - 4) * U[i - 3] + U[i - 4]);

    row[0] = 1;
    for (m = 0; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        if (m > 0) {
            for (k = m; k >= 1; k--)
                row[k] += row[k - 1];
        }
        for (k = 0; 2 * k <= m; k++) {
            idx_t j = m - 2 * k;
            seq_t t = row[k] * D[m - k] * D[k];
            if (j == 1)
                neg += t;
            else
                pos += t * U[j];
        }
        K[m] = pos - neg;
    }

    B[0] = 0;
    for (m = 1; m <= N; m++)
        B[m] = K[m] + (seq_t)(2 * (m - 1)) * B[m - 1];

    a1627_D = D;
    a1627_U = U;
    a1627_row = row;
    a1627_K = K;
    a1627_B = B;
    a1627_cap = N + 1;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    if (n >= a1627_cap) {
        idx_t N = 2 * a1627_cap;
        if (N < n) N = n;
        if (N < 16) N = 16;
        a1627_build(N);
    }
    return a1627_K[n] + a1627_K[n - 1] + (seq_t)(3 * (n - 1)) * a1627_B[n - 1];
}
