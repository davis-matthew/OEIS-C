/*
 * A000323  Let A(n) = #{(i,j): i^2 + j^2 <= n}, V(n) = Pi*n, P(n) = A(n) -
 *          V(n); A000099 gives values of n where |P(n)| sets a new record;
 *          sequence gives A(A000099(n)).
 * https://oeis.org/A000323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: scan m = 1, 2, 3, ..., keeping A(m) = #{(i,j): i^2+j^2
 *            <= m} as A(m) = A(m-1) + r2(m), with r2 sieved in blocks over
 *            the lattice points. Whenever |A(m) - Pi*m| sets a new record,
 *            A(m) is the next term. Pi*m is compared in 64.64 fixed point
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   isqrt_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_local.h"

#define BLK ((uint64_t)1 << 18)

static uint64_t *recs = 0;
static size_t nrec = 0, caprec = 0;
static uint64_t next_m = 1;     /* next m to process */
static uint64_t curA = 1;       /* A(next_m - 1) */
static unsigned __int128 best = 0;
static uint16_t *cnt = 0;

static void push_rec(uint64_t v)
{
    if (nrec == caprec) {
        caprec = caprec ? caprec * 2 : 256;
        recs = (uint64_t *)realloc(recs, caprec * sizeof(uint64_t));
    }
    recs[nrec++] = v;
}

static void process_block(void)
{
    uint64_t L = next_m, R = next_m + BLK, i, m;
    /* pi * 2^64 */
    const unsigned __int128 PI64 =
        ((unsigned __int128)0x3243F6A8885A308DULL << 4) | 3;
    if (!cnt) cnt = (uint16_t *)malloc(BLK * sizeof(uint16_t));
    memset(cnt, 0, BLK * sizeof(uint16_t));
    /* quadrant points i >= 1, j >= 0 with i^2 + j^2 in [L, R) */
    for (i = 1; i * i < R; i++) {
        uint64_t ii = i * i, j, q;
        if (ii >= L) j = 0;
        else {
            uint64_t t = L - ii;
            j = isqrt_local(t);
            if (j * j < t) j++;
        }
        q = ii + j * j;
        while (q < R) {
            cnt[q - L]++;
            q += 2 * j + 1;
            j++;
        }
    }
    for (m = L; m < R; m++) {
        unsigned __int128 a, v, d;
        curA += 4 * (uint64_t)cnt[m - L];
        a = (unsigned __int128)curA << 64;
        v = (unsigned __int128)m * PI64;
        d = a > v ? a - v : v - a;
        if (d > best) {
            best = d;
            push_rec(curA);
        }
    }
    next_m = R;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (nrec < n) process_block();
    return (seq_t)recs[n - 1];
}
