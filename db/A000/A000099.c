/*
 * A000099  Let A(n) = #{(i,j): i^2 + j^2 <= n}, V(n) = Pi*n, P(n) = A(n) -
 *          V(n); sequence gives values of n where |P(n)| sets a new record.
 * https://oeis.org/A000099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Let A(m) = #{(i,j): i^2 + j^2 <= m}. Sieve r2(m) block
 *            by block, accumulate A(m), and compare |A(m) - Pi*m| using exact
 *            fixed-point arithmetic (Pi scaled by 2^62). a(n) is the n-th m
 *            >= 1 where the value sets a new strict record; the starting
 *            record is |P(0)| = 1
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef __int128 a99_i128;
typedef unsigned __int128 a99_u128;

/* round(Pi * 2^62) */
#define A99_PI62 ((uint64_t)0xC90FDAA22168C235ULL)
#define A99_BS 65536

static uint64_t *a99_rec = 0;
static size_t a99_nrec = 0, a99_caprec = 0;
static uint64_t *a99_jn = 0;          /* a99_jn[i]: next j to use for row i */
static size_t a99_nj = 0, a99_capj = 0;
static uint64_t a99_L = 1;            /* start of the next block */
static a99_i128 a99_A = 1;            /* A(a99_L - 1) */
static a99_u128 a99_best = (a99_u128)1 << 62;   /* |P(0)| * 2^62 */
static uint32_t a99_cnt[A99_BS];

static void a99_push_rec(uint64_t m)
{
    if (a99_nrec == a99_caprec) {
        a99_caprec = a99_caprec ? 2 * a99_caprec : 256;
        a99_rec = (uint64_t *)realloc(a99_rec, a99_caprec * sizeof(uint64_t));
    }
    a99_rec[a99_nrec++] = m;
}

static void a99_block(void)
{
    uint64_t L = a99_L, R = L + A99_BS;
    size_t i, k;
    memset(a99_cnt, 0, sizeof(a99_cnt));
    while ((uint64_t)a99_nj * (uint64_t)a99_nj < R) {
        if (a99_nj == a99_capj) {
            a99_capj = a99_capj ? 2 * a99_capj : 1024;
            a99_jn = (uint64_t *)realloc(a99_jn, a99_capj * sizeof(uint64_t));
        }
        a99_jn[a99_nj++] = 0;
    }
    for (i = 0; i < a99_nj; i++) {
        uint64_t i2 = (uint64_t)i * (uint64_t)i;
        uint64_t j = a99_jn[i];
        uint64_t s;
        while (i2 + j * j < L) j++;
        while ((s = i2 + j * j) < R) {
            a99_cnt[s - L] += (uint32_t)((i ? 2 : 1) * (j ? 2 : 1));
            j++;
        }
        a99_jn[i] = j;
    }
    for (k = 0; k < A99_BS; k++) {
        uint64_t m = L + k;
        a99_i128 P;
        a99_u128 aP;
        a99_A += a99_cnt[k];
        P = (a99_A << 62) - (a99_i128)m * (a99_i128)A99_PI62;
        aP = (a99_u128)(P < 0 ? -P : P);
        if (aP > a99_best) {
            a99_best = aP;
            a99_push_rec(m);
        }
    }
    a99_L = R;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (a99_nrec < (size_t)n) a99_block();
    return (seq_t)a99_rec[n - 1];
}
