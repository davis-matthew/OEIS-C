/*
 * A000092  Let A(n) = #{(i,j,k): i^2 + j^2 + k^2 <= n}, V(n) =
 *          (4/3)Pi*n^(3/2), P(n) = A(n) - V(n); sequence gives values of n
 *          where |P(n)| sets a new record.
 * https://oeis.org/A000092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th N >= 1 at which |P(N)| = |A(N) -
 *            (4/3)*Pi*N^(3/2)| sets a new record. A(N) = A(N-1) + r3(N), A(0)
 *            = 1, with r3(N) = Sum_k r2(N - k^2); r2 is stored split by
 *            residue mod 8, skipping the classes 3, 6 and 7 mod 8, where r2 =
 *            0
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LBLK 4096u                     /* q-block length: covers 8*LBLK values of N */

static int16_t *R[8];                  /* R[c][p] = r2(8p + c), only for c in {0,1,2,4,5} */
static uint64_t Pcap = 0;              /* R arrays cover m < 8*Pcap */
static uint64_t Qdone = 0;             /* all N < 8*Qdone processed */
static int64_t acc = 1;                /* A(8*Qdone - 1), A(0) = 1 */
static long double best = -1.0L;
static uint64_t *rec_list = NULL;
static size_t rec_cnt = 0, rec_cap = 0;
static int32_t dst[8][LBLK];

static int zero_class(unsigned r) { return r == 3 || r == 6 || r == 7; }

static void push_rec(uint64_t v)
{
    if (rec_cnt == rec_cap) {
        rec_cap = rec_cap ? rec_cap * 2 : 256;
        rec_list = (uint64_t *)realloc(rec_list, rec_cap * sizeof(uint64_t));
    }
    rec_list[rec_cnt++] = v;
}

static void build_r2(uint64_t P)
{
    unsigned c;
    uint64_t i, j, Mx = 8 * P - 1;
    for (c = 0; c < 8; c++) {
        free(R[c]);
        R[c] = NULL;
        if (!zero_class(c))
            R[c] = (int16_t *)calloc((size_t)P, sizeof(int16_t));
    }
    for (i = 0; 2 * i * i <= Mx; i++) {
        for (j = i; i * i + j * j <= Mx; j++) {
            uint64_t m = i * i + j * j;
            int w = (i ? 2 : 1) * (j ? 2 : 1) * (i == j ? 1 : 2);
            R[m & 7][m >> 3] += (int16_t)w;
        }
    }
    Pcap = P;
}

static void do_block(void)
{
    const long double C = 4.0L * 3.14159265358979323846264338327950288L / 3.0L;
    uint64_t Q0 = Qdone, hiq = Q0 + LBLK, Nmax = 8 * hiq - 1, k, qi;
    unsigned c;

    if (hiq > Pcap) {
        uint64_t np = Pcap + Pcap / 2;
        if (np < hiq) np = hiq;
        np = (np + LBLK - 1) / LBLK * LBLK;
        build_r2(np);
    }
    memset(dst, 0, sizeof(dst));

    for (k = 1; k * k <= Nmax; k++) {
        uint64_t k2 = k * k, t = k2 >> 3;
        unsigned s = (unsigned)(k2 & 7);
        for (c = 0; c < 7; c++) {
            uint64_t off = t + (c < s ? 1 : 0), lo, len, x;
            unsigned rr = (c + 8 - s) & 7;
            int32_t *d;
            const int16_t *src;
            if (zero_class(rr)) continue;
            lo = Q0 > off ? Q0 : off;
            if (lo >= hiq) continue;
            len = hiq - lo;
            d = dst[c] + (lo - Q0);
            src = R[rr] + (lo - off);
            for (x = 0; x < len; x++)
                d[x] += src[x];
        }
    }

    for (qi = 0; qi < LBLK; qi++) {
        uint64_t q = Q0 + qi;
        for (c = 0; c < 8; c++) {
            uint64_t N = 8 * q + c;
            int64_t r3;
            long double ld, p;
            if (N == 0) continue;
            r3 = (c == 7) ? 0 : 2 * (int64_t)dst[c][qi] + (R[c] ? R[c][q] : 0);
            acc += r3;
            ld = (long double)N;
            p = (long double)acc - C * ld * sqrtl(ld);
            if (p < 0) p = -p;
            if (p > best) {
                best = p;
                push_rec(N);
            }
        }
    }
    Qdone = hiq;
}

seq_t a(idx_t n)
{
    while ((uint64_t)rec_cnt < (uint64_t)n)
        do_block();
    return (seq_t)rec_list[n - 1];
}
