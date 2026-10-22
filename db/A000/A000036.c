/*
 * A000036  Let A(n) = #{(i,j): i^2 + j^2 <= n}, V(n) = Pi*n, P(n) = A(n) -
 *          V(n); A000099 gives values of n where |P(n)| sets a new record;
 *          sequence gives closest integer to P(A000099(n)).
 * https://oeis.org/A000036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   search, not a closed form. Walk m = 1, 2, ... and keep the
 *            running lattice count A(m) = A057655(m). The increments r2(m)
 *            come from a block sieve over i^2 + j^2. P(m) = A(m) - Pi*m is
 *            held as an exact 64-bit fixed-point value. Each time |P(m)|
 *            beats the previous record, round(P(m)) is the next ter...
 * Range:     i8 n <= 69, i16 n <= 200, i32 n <= 200, i64 n <= 200,
 *            i128 n <= 200, big any n
 * Helpers:   isr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isr.h"

typedef unsigned __int128 u128_t;
typedef __int128 s128_t;

static uint64_t st_L = 1;          /* next m to process */
static uint64_t st_A = 1;          /* A(st_L - 1); A(0) = 1 */
static bool st_have = false;
static u128_t st_rec = 0;          /* record |P| in 2^-64 units */
static long long *st_res = NULL;
static size_t st_cnt = 0, st_cap = 0;
static uint16_t *st_buf = NULL;
static size_t st_bufcap = 0;

static void push_res(long long v)
{
    if (st_cnt == st_cap) {
        st_cap = st_cap ? 2 * st_cap : 64;
        st_res = (long long *)realloc(st_res, st_cap * sizeof(long long));
    }
    st_res[st_cnt++] = v;
}

static void run_block(void)
{
    const u128_t PI64 = ((u128_t)3 << 64) | (u128_t)0x243F6A8885A308D3ULL;
    uint64_t L = st_L;
    uint64_t B = 2 * isr(L);
    uint64_t H, i, k;
    if (B < 65536) B = 65536;
    if (st_bufcap < B) {
        st_bufcap = (size_t)B;
        st_buf = (uint16_t *)realloc(st_buf, st_bufcap * sizeof(uint16_t));
    }
    memset(st_buf, 0, (size_t)B * sizeof(uint16_t));
    H = L + B;
    /* quadrant count: i >= 1, j >= 0; r2(m) = 4 * count */
    for (i = 1; i * i < H; i++) {
        uint64_t i2 = i * i, j;
        if (i2 >= L) j = 0;
        else {
            uint64_t d = L - i2;
            j = isr(d);
            if (j * j < d) j++;
        }
        for (; i2 + j * j < H; j++) st_buf[i2 + j * j - L]++;
    }
    for (k = 0; k < B; k++) {
        uint64_t m = L + k;
        u128_t a64, v64, mag;
        bool neg;
        st_A += 4 * (uint64_t)st_buf[k];
        a64 = (u128_t)st_A << 64;
        v64 = (u128_t)m * PI64;
        if (a64 >= v64) { mag = a64 - v64; neg = false; }
        else { mag = v64 - a64; neg = true; }
        if (!st_have || mag > st_rec) {
            long long r = (long long)((mag + ((u128_t)1 << 63)) >> 64);
            st_have = true;
            st_rec = mag;
            push_res(neg ? -r : r);
        }
    }
    st_L = H;
}

seq_t a(idx_t n)
{
    while (st_cnt < (size_t)n) run_block();
    return (seq_t)st_res[n - 1];
}
