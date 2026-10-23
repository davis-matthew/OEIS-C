/*
 * A100613  Number of elements in the set {(x,y): 1 <= x,y <= n, gcd(x,y) >
 *          1}.
 * https://oeis.org/A100613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*n - 2*SumPhi(n) + 1, where SumPhi(n) = sum_{k=1}^n
 *            phi(k) and phi(k) is Euler's totient function
 * Range:     u8 n <= 26, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

static seq_t *phi = NULL;      /* phi[i] = totient of i */
static seq_t *sumPhi = NULL;   /* sumPhi[i] = sum_{k=1}^i phi(k) */
static size_t maxN = 0;        /* current maximum n for which arrays are valid */

static void rebuild_phi(size_t n)
{
    size_t i, j;
    size_t newCap = maxN == 0 ? n + 1 : maxN;
    while (newCap <= n) newCap *= 2;

    phi = (seq_t *)realloc(phi, newCap * sizeof(seq_t));
    sumPhi = (seq_t *)realloc(sumPhi, newCap * sizeof(seq_t));
    if (!phi || !sumPhi) { /* memory error; abort */
        exit(1);
    }

    /* initialize phi[i] = i */
    for (i = 0; i < newCap; ++i) phi[i] = (seq_t)i;

    /* linear sieve for phi up to newCap-1 */
    for (i = 2; i < newCap; ++i) {
        if (phi[i] == (seq_t)i) {            /* i is prime */
            for (j = i; j < newCap; j += i) {
                phi[j] -= phi[j] / (seq_t)i;
            }
        }
    }

    /* compute prefix sums */
    sumPhi[0] = 0;
    for (i = 1; i < newCap; ++i) {
        sumPhi[i] = sumPhi[i - 1] + phi[i];
    }

    maxN = newCap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not required but safe */

    if ((size_t)n >= maxN) rebuild_phi((size_t)n + 1);

    seq_t n_seq = (seq_t)n;
    seq_t result = n_seq * n_seq - (seq_t)2 * sumPhi[n] + (seq_t)1;
    return result;
}
