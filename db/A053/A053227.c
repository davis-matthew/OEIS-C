/*
 * A053227  First differences of sigma(n) that are negative.
 * https://oeis.org/A053227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th absolute value of a negative first difference of
 *            σ(n)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- helper: integer square root for uint64_t ---------- */
#include <stdint.h>
#include <stdlib.h>

static idx_t isqrt_u64(idx_t v)
{
    /* floor(sqrt(v))  using binary search on 64‑bit space */
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* sqrt(2^64‑1) < 2^32 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ---------- divisor sum σ(n) ---------- */
static seq_t sigma_of(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                       /* σ(1) = 1 */

    unsigned __int128 sig = 1;                 /* running product of prime factor sums */
    idx_t rem = n;

    /* factor 2 separately */
    if ((rem & (idx_t)1) == 0) {
        unsigned __int128 pow = 1, sum = 1;
        while ((rem & (idx_t)1) == 0) {
            rem >>= 1;
            pow *= (unsigned __int128)2;
            sum += pow;
        }
        sig *= sum;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= isqrt_u64(rem); p += 2) {
        if (rem % p == 0) {
            unsigned __int128 pow = 1, sum = 1;
            while (rem % p == 0) {
                rem /= p;
                pow *= (unsigned __int128)p;
                sum += pow;
            }
            sig *= sum;
        }
    }

    /* remaining prime > sqrt(n) */
    if (rem > 1)
        sig *= ((unsigned __int128)1 + rem);

    return (seq_t)sig;
}

/* ---------- the sequence function ---------- */
seq_t a(idx_t n)
{
    /* state shared across calls */
    static idx_t cur_num = 1;                /* last processed integer */
    static seq_t prev_sigma = (seq_t)1;      /* σ(cur_num) */
    static idx_t  neg_cnt  = 0;              /* number of stored negative diffs */
    static seq_t *neg_buf = NULL;            /* array of differences */
    static size_t neg_cap  = 0;              /* capacity of neg_buf */

    /* collect negative differences until we have at least 'n' of them */
    while (neg_cnt < n) {
        ++cur_num;                           /* next integer */
        seq_t cur_sigma = sigma_of(cur_num);

        if (cur_sigma < prev_sigma) {        /* negative difference */
            seq_t diff = prev_sigma - cur_sigma;   /* positive magnitude */

            if (neg_cnt >= neg_cap) {
                neg_cap = neg_cap ? (neg_cap << 1) : 16;
                neg_buf = (seq_t *)realloc(neg_buf, neg_cap * sizeof(seq_t));
            }
            neg_buf[neg_cnt++] = diff;
        }

        prev_sigma = cur_sigma;
    }

    return neg_buf[n - 1];
}
