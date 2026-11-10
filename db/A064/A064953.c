/*
 * A064953  Where the even terms appear in A064413.
 * https://oeis.org/A064953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th even term of the EKG sequence A064413
 *            (computed by generating EKG: a(1)=1,a(2)=2, then smallest unused
 *            number sharing a factor with the previous term)
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9291 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static unsigned char *A064953_used = NULL;
static uint32_t *A064953_ptr = NULL;
static size_t A064953_cap = 0;
static uint64_t *A064953_ev = NULL;
static size_t A064953_evn = 0, A064953_evcap = 0;
static uint64_t A064953_k = 0;
static uint32_t A064953_last = 0;

static void A064953_grow(size_t v)
{
    if (v < A064953_cap) return;
    size_t nc = A064953_cap ? A064953_cap : 1024;
    while (nc <= v) nc *= 2;
    A064953_used = (unsigned char *)realloc(A064953_used, nc);
    A064953_ptr = (uint32_t *)realloc(A064953_ptr, nc * sizeof(uint32_t));
    if (!A064953_used || !A064953_ptr) exit(1);
    memset(A064953_used + A064953_cap, 0, nc - A064953_cap);
    memset(A064953_ptr + A064953_cap, 0, (nc - A064953_cap) * sizeof(uint32_t));
    A064953_cap = nc;
}

static void A064953_push(uint64_t idx)
{
    if (A064953_evn >= A064953_evcap) {
        A064953_evcap = A064953_evcap ? A064953_evcap * 2 : 1024;
        A064953_ev = (uint64_t *)realloc(A064953_ev, A064953_evcap * sizeof(uint64_t));
        if (!A064953_ev) exit(1);
    }
    A064953_ev[A064953_evn++] = idx;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A064953_k == 0) {
        A064953_grow(1024);
        A064953_used[1] = 1;
        A064953_used[2] = 1;
        A064953_k = 2;
        A064953_last = 2;
        A064953_push(2);
    }
    while (A064953_evn < (size_t)n) {
        uint32_t x = A064953_last;
        uint32_t best = 0;
        uint32_t p = 2;
        while (x > 1) {
            if (x % p == 0) {
                while (x % p == 0) x /= p;
                uint32_t c = A064953_ptr[p] ? A064953_ptr[p] : p;
                for (;;) {
                    A064953_grow(c);
                    if (!A064953_used[c]) break;
                    c += p;
                }
                A064953_ptr[p] = c;
                if (best == 0 || c < best) best = c;
            }
            if (p * p > x && x > 1) p = x; else p++;
        }
        A064953_grow(best);
        A064953_used[best] = 1;
        A064953_last = best;
        A064953_k++;
        if ((best & 1u) == 0) A064953_push(A064953_k);
    }
    return (seq_t)A064953_ev[n - 1];
}
