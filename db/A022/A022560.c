/*
 * A022560  a(0)=0, a(2*n) = 2*a(n) + 2*a(n-1) + n^2 + n, a(2*n+1) = 4*a(n) +
 *          (n+1)^2.
 * https://oeis.org/A022560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 182, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0,
   a(1) = 1,
   a(2*n)   = 2*a(n) + 2*a(n-1) + n*n + n,
   a(2*n+1) = 4*a(n) + (n+1)*(n+1)   for n ≥ 0
*/
#include <stdint.h>
#include <stddef.h>

#define MAX_CACHE 20001   /* cache sufficient for n up to ≈10 000 */

static seq_t cache[MAX_CACHE];
static unsigned char computed[MAX_CACHE] = {0};

static seq_t compute(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (n < MAX_CACHE && computed[(size_t)n]) return cache[(size_t)n];

    seq_t ret;
    if ((n & 1) == 0) {                    /* even */
        idx_t k = n >> 1;
        seq_t a_k   = compute(k);
        seq_t a_k_1 = compute(k - 1);
        seq_t kseq  = (seq_t)k;
        ret = (seq_t)2 * a_k + (seq_t)2 * a_k_1 + kseq * (kseq + (seq_t)1);
    } else {                               /* odd */
        idx_t k = (n - 1) >> 1;
        seq_t a_k  = compute(k);
        seq_t kp1  = (seq_t)(k + 1);
        ret = (seq_t)4 * a_k + kp1 * kp1;
    }

    if (n < MAX_CACHE) {
        cache[(size_t)n] = ret;
        computed[(size_t)n] = 1;
    }

    return ret;
}

seq_t a(idx_t n)
{
    return compute(n);
}
