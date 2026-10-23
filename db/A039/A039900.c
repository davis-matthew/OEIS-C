/*
 * A039900  Number of partitions satisfying 0 < cn(0,5) + cn(1,5) + cn(4,5).
 * https://oeis.org/A039900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - A003106(n)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n having at least one part ≡ 0,1 or 4 (mod 5) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *p_arr = NULL;   /* partitions of n (A000041) */
static seq_t *q_arr = NULL;   /* partitions using parts ≡ 2 or 3 (mod 5) (A003106) */
static idx_t  p_cap = 0;      /* current capacity of arrays (number of elements) */

/* Extend both arrays up to at least N (inclusive) */
static void extend_seq(idx_t N)
{
    if (N + 1 <= p_cap) return;                 /* already computed */

    idx_t len = N + 1;

    seq_t *p_new = (seq_t *)calloc((size_t)len, sizeof(seq_t));
    seq_t *q_new = (seq_t *)calloc((size_t)len, sizeof(seq_t));

    if (!p_new || !q_new) {                     /* allocation failed */
        /* In a real program you may want to handle this more gracefully */
        abort();
    }

    /* --- Partitions p[n] (A000041) using pentagonal recurrence --- */
    p_new[0] = (seq_t)1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            if (k & 1U)  sum += p_new[n - pent1];
            else        sum -= p_new[n - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            if (k & 1U)  sum += p_new[n - pent2];
            else        sum -= p_new[n - pent2];
        }
        p_new[n] = sum;
    }

    /* --- Partitions q[n] with parts ≡ 2 or 3 (mod 5) (A003106) --- */
    q_new[0] = (seq_t)1;
    for (idx_t j = 1; ; ++j) {
        idx_t s = 5 * j - 3;                     /* part size 5j-3 (≡2 mod 5) */
        if (s > N) break;
        for (idx_t n = s; n <= N; ++n) {
            q_new[n] += q_new[n - s];
        }
    }
    for (idx_t j = 1; ; ++j) {
        idx_t s = 5 * j - 2;                     /* part size 5j-2 (≡3 mod 5) */
        if (s > N) break;
        for (idx_t n = s; n <= N; ++n) {
            q_new[n] += q_new[n - s];
        }
    }

    /* Replace old arrays with the new ones */
    free(p_arr);
    free(q_arr);
    p_arr = p_new;
    q_arr = q_new;
    p_cap = (size_t)len;
}

/* Sequence function */
seq_t a(idx_t n)
{
    extend_seq(n);
    return p_arr[n] - q_arr[n];   /* For n=0 both are 1, giving 0 as required */
}
