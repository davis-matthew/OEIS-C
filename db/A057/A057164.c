/*
 * A057164  Self-inverse permutation of natural numbers induced by reflections
 *          of the rooted plane trees and mountain ranges encoded by A014486.
 * https://oeis.org/A057164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080300(A036044(A014486(n))): unrank the n-th Dyck word,
 *            reflect it (complement and reverse its bits), then rank it
 *            again. a(0) = 0
 * Range:     u8 n <= 197, u16 n <= 6917, u32 n <= 6917, u64 n <= 6917,
 *            u128 n <= 6917, big any n
 * Validated: standard, pass, 87 terms through n = 6775 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A057164_W[66][68];
static int A057164_init = 0;

static void A057164_setup(void)
{
    int r, h;
    if (A057164_init) return;
    for (r = 0; r < 66; r++)
        for (h = 0; h < 68; h++)
            A057164_W[r][h] = 0;
    A057164_W[0][0] = 1;
    for (r = 1; r < 66; r++)
        for (h = 0; h < 67; h++) {
            uint64_t v = A057164_W[r - 1][h + 1];
            if (h > 0) v += A057164_W[r - 1][h - 1];
            A057164_W[r][h] = v;
        }
    A057164_init = 1;
}

seq_t a(idx_t n)
{
    A057164_setup();
    if (n == 0) return (seq_t)0;

    uint64_t m = (uint64_t)n - 1;
    int k = 1;
    uint64_t base = 1; /* index of first word of length 2k */
    while (k < 32) {
        uint64_t c = A057164_W[2 * k][0];
        if (m < c) break;
        m -= c;
        base += c;
        k++;
    }

    int L = 2 * k;
    uint64_t w = 0;
    int h = 0, pos;
    for (pos = 0; pos < L; pos++) {
        int rem = L - pos - 1;
        if (h > 0) {
            uint64_t cnt = A057164_W[rem][h - 1];
            if (m < cnt) {
                w <<= 1;
                h--;
                continue;
            }
            m -= cnt;
        }
        w = (w << 1) | 1;
        h++;
    }

    /* reflect: complement and reverse L bits */
    uint64_t r = 0;
    uint64_t v = w;
    for (pos = 0; pos < L; pos++) {
        r = (r << 1) | ((v & 1) ^ 1);
        v >>= 1;
    }

    /* rank */
    uint64_t rank = 0;
    h = 0;
    for (pos = 0; pos < L; pos++) {
        int rem = L - pos - 1;
        int bit = (int)((r >> (L - 1 - pos)) & 1);
        if (bit) {
            if (h > 0) rank += A057164_W[rem][h - 1];
            h++;
        } else {
            h--;
        }
    }

    return (seq_t)(base + rank);
}
