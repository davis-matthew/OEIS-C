/*
 * A132993  Triangle t(n,m) = P(n-m+1) * P(m+1) read by rows, 0<=m<=n, where
 *          P=A000041 are the partition numbers.
 * https://oeis.org/A132993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A132993(n) = P(r − m + 1) * P(m + 1) where r = ⌊(√(8n+1) − 1) /
 *            2⌋ and m = n − r(r+1)/2. P(k) denotes the k‑th integer partition
 *            number
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cached partition numbers P[0..] */
static seq_t *P = NULL;        /* array of partition numbers */
static idx_t  Pmax = 0;        /* highest index computed (valid only when P ≠ NULL) */
static size_t Pcap = 0;        /* allocated capacity of P */

/* Ensure that partition numbers P[0..up_to] are computed. */
static void ensure_partitions(idx_t up_to)
{
    /* Nothing to do if we already have enough */
    if (P != NULL && up_to <= Pmax)
        return;

    size_t new_cap = (size_t)up_to + 1;

    if (P == NULL) {
        /* First allocation */
        P = seq_alloc(new_cap);
        Pcap = new_cap;
        if (up_to >= 0) {
            P[0] = 1;        /* partition(0) = 1 */
            Pmax = 0;
        }
    } else if (new_cap > Pcap) {
        /* Grow the array */
        P = seq_realloc(P, Pcap, new_cap);
        Pcap = new_cap;
    }

    /* Compute the missing partition values */
    for (idx_t i = Pmax + 1; i <= up_to; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t k1 = k * (3 * k - 1) / 2;
            if (k1 > i) break;
            if (k & 1)
                sum += P[i - k1];
            else
                sum -= P[i - k1];

            idx_t k2 = k * (3 * k + 1) / 2;
            if (k2 > i) continue;
            if (k & 1)
                sum += P[i - k2];
            else
                sum -= P[i - k2];
        }
        P[i] = sum;
    }
    Pmax = up_to;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high == 0) {          /* overflow guard */
            high = n + 1;
            break;
        }
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* Column index m */
    idx_t m = n - (idx_t)((__uint128_t)r * (r + 1) / 2);

    /* Ensure partition numbers up to r+1 are available */
    ensure_partitions(r + 1);

    /* Indices into the partition array */
    idx_t idx_a = r - m + 1;     /* 0 ≤ idx_a ≤ r+1 */
    idx_t idx_b = m + 1;          /* 0 ≤ idx_b ≤ r+1 */

    seq_t prod = P[idx_a];
    prod *= P[idx_b];
    return prod;
}
