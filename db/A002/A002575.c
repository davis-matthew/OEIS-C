/*
 * A002575  Coefficients of Bell's formula for making change.
 * https://oeis.org/A002575
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002577(n-1) - 1, where A002577(m) = number of binary
 *            partitions of 2^m. Let g_k(q) = number of partitions of q*2^k
 *            into powers of 2 that are <= 2^k. Then g_0(q) = 1 and g_{k+1}(q)
 *            = Sum_{j=0..q} g_k(2j). Each g_k is a polynomial of degree k, so
 *            it is stored as its forward differences at 0...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002575_MAXK 10010

static seq_t A002575_D[A002575_MAXK + 3]; /* forward differences of g_cur at 0 */
static seq_t A002575_W[A002575_MAXK + 3]; /* work copy while stepping */
static seq_t A002575_G[A002575_MAXK + 3]; /* values of g_{cur+1} at 0..cur+1 */
static seq_t A002575_R[A002575_MAXK + 3]; /* R[k] = g_k(1) */
static long long A002575_cur = -1;

static void A002575_level_up(void)
{
    long long k = A002575_cur, i, j, x;
    seq_t sum = 0;
    for (i = 0; i <= k; i++)
        A002575_W[i] = A002575_D[i];
    /* evaluate g_k at x = 0..2k+2, prefix-summing the values at even x */
    for (x = 0; x <= 2 * k + 2; x++) {
        if ((x & 1) == 0) {
            sum += A002575_W[0];
            A002575_G[x / 2] = sum;
        }
        if (x < 2 * k + 2)
            for (i = 0; i < k; i++)
                A002575_W[i] += A002575_W[i + 1];
    }
    /* G now holds g_{k+1}(0..k+1); turn it into forward differences (all nonnegative) */
    for (j = 1; j <= k + 1; j++)
        for (i = k + 1; i >= j; i--)
            A002575_G[i] = A002575_G[i] - A002575_G[i - 1];
    for (i = 0; i <= k + 1; i++)
        A002575_D[i] = A002575_G[i];
    A002575_cur = k + 1;
    A002575_R[k + 1] = A002575_D[0] + A002575_D[1];
}

seq_t a(idx_t n)
{
    long long m;
    if (n < 2) return 0;
    m = (long long)(n - 1);
    if (m > A002575_MAXK) return 0;
    if (A002575_cur < 0) {
        A002575_D[0] = 1;
        A002575_cur = 0;
        A002575_R[0] = 1;
    }
    while (A002575_cur < m)
        A002575_level_up();
    return A002575_R[m] - 1;
}
