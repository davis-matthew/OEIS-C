/*
 * A064474  Number of even terms among first n terms of A064413.
 * https://oeis.org/A064474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even terms among first n terms of the EKG
 *            sequence A064413 (a(1)=1, a(2)=2, a(n)=smallest unused number
 *            sharing a factor with a(n-1)); computed by direct simulation
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A064474_used = NULL;
static uint32_t *A064474_ptr = NULL;
static size_t A064474_cap = 0;
static uint32_t *A064474_cnt = NULL;   /* A064474_cnt[i] = evens among first i terms */
static size_t A064474_len = 0, A064474_ccap = 0;
static uint32_t A064474_last = 0;

static void A064474_ensure(size_t v)
{
    if (v < A064474_cap) return;
    size_t nc = A064474_cap ? A064474_cap : 1024;
    while (nc <= v) nc *= 2;
    A064474_used = (unsigned char *)realloc(A064474_used, nc);
    A064474_ptr = (uint32_t *)realloc(A064474_ptr, nc * sizeof(uint32_t));
    if (!A064474_used || !A064474_ptr) exit(1);
    memset(A064474_used + A064474_cap, 0, nc - A064474_cap);
    memset(A064474_ptr + A064474_cap, 0, (nc - A064474_cap) * sizeof(uint32_t));
    A064474_cap = nc;
}

static void A064474_push(uint32_t v)
{
    if (A064474_len + 1 >= A064474_ccap) {
        A064474_ccap = A064474_ccap ? A064474_ccap * 2 : 1024;
        A064474_cnt = (uint32_t *)realloc(A064474_cnt, A064474_ccap * sizeof(uint32_t));
        if (!A064474_cnt) exit(1);
    }
    A064474_ensure(v);
    A064474_used[v] = 1;
    A064474_cnt[A064474_len + 1] = A064474_cnt[A064474_len] + ((v & 1) ? 0 : 1);
    A064474_len++;
    A064474_last = v;
}

static void A064474_step(void)
{
    uint32_t x = A064474_last, best = 0;
    uint32_t p;
    for (p = 2; (uint64_t)p * p <= x || x > 1; p++) {
        if (x % p) {
            if ((uint64_t)p * p > x) p = x - 1; /* x is prime remainder; jump */
            continue;
        }
        while (x % p == 0) x /= p;
        {
            A064474_ensure(p);
            uint32_t c = A064474_ptr[p] ? A064474_ptr[p] : p;
            for (;;) {
                A064474_ensure(c);
                if (!A064474_used[c]) break;
                c += p;
            }
            A064474_ptr[p] = c;
            if (!best || c < best) best = c;
        }
        if (x == 1) break;
    }
    A064474_push(best);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A064474_len == 0) {
        A064474_ensure(16);
        A064474_ccap = 1024;
        A064474_cnt = (uint32_t *)calloc(A064474_ccap, sizeof(uint32_t));
        if (!A064474_cnt) exit(1);
        A064474_push(1);
        A064474_push(2);
    }
    while ((idx_t)A064474_len < n) A064474_step();
    return (seq_t)A064474_cnt[(size_t)n];
}
