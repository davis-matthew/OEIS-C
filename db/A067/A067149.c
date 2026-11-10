/*
 * A067149  Indices of records of A067148.
 * https://oeis.org/A067149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least m with A067148(m) = n-1 (first record index);
 *            computed by a segmented sieve counting pairs (F_t + F_{t+1}*x,
 *            F_{t-1} + F_t*x), skipping duplicate x=1 cases for t>=2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A067149_SEG (1u << 22)

static uint8_t A067149_cnt[A067149_SEG];
static uint64_t A067149_rec[64];
static int A067149_nrec = 0;
static uint64_t A067149_next = 1;

static void A067149_scan(void)
{
    uint64_t lo = A067149_next, hi = lo + A067149_SEG;
    uint64_t i;
    uint64_t fp = 0, ft = 1, ft1 = 1;
    int t = 1;
    for (i = 0; i < A067149_SEG; i++) A067149_cnt[i] = 0;
    while (ft < hi) {
        uint64_t xmin = (t == 1) ? 1 : 2;
        uint64_t m = ft + ft1 * xmin;
        if (m < lo) {
            uint64_t steps = (lo - m + ft1 - 1) / ft1;
            m += steps * ft1;
        }
        for (; m < hi; m += ft1) A067149_cnt[m - lo]++;
        {
            uint64_t nf = ft + ft1;
            fp = ft;
            ft = ft1;
            ft1 = nf;
        }
        t++;
    }
    (void)fp;
    for (i = 0; i < A067149_SEG; i++) {
        uint64_t c = A067149_cnt[i];
        while ((uint64_t)A067149_nrec <= c && A067149_nrec < 64) {
            A067149_rec[A067149_nrec++] = lo + i;
        }
    }
    A067149_next = hi;
}

seq_t a(idx_t n)
{
    int k = (int)(n - 1);
    if (A067149_nrec == 0) {
        A067149_rec[0] = 1;
        A067149_nrec = 1;
    }
    while (A067149_nrec <= k) A067149_scan();
    return (seq_t)A067149_rec[k];
}
