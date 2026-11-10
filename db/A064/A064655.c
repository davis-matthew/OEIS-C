/*
 * A064655  Length of n-th run of even numbers in A064413.
 * https://oeis.org/A064655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of n-th maximal run of even terms in the EKG
 *            sequence A064413 (computed by generating the EKG sequence)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9335 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define A064655_LIM 2000000u

static uint32_t A064655_spf[A064655_LIM + 1];
static uint32_t A064655_nxt[A064655_LIM + 1];
static unsigned char A064655_used[A064655_LIM + 1];
static uint32_t *A064655_runs = NULL;
static size_t A064655_cnt = 0, A064655_cap = 0;
static uint32_t A064655_last = 0, A064655_cur = 0;
static bool A064655_init = false;

static void A064655_setup(void)
{
    uint32_t i, j;
    for (i = 2; i <= A064655_LIM; i++) {
        if (A064655_spf[i] == 0) {
            for (j = i; j <= A064655_LIM; j += i)
                if (A064655_spf[j] == 0) A064655_spf[j] = i;
        }
    }
    A064655_used[1] = 1;
    A064655_used[2] = 1;
    A064655_last = 2;
    A064655_cur = 1;
    A064655_init = true;
}

static void A064655_push(uint32_t v)
{
    if (A064655_cnt == A064655_cap) {
        A064655_cap = A064655_cap ? A064655_cap * 2 : 1024;
        A064655_runs = (uint32_t *)realloc(A064655_runs, A064655_cap * sizeof(uint32_t));
        if (!A064655_runs) exit(1);
    }
    A064655_runs[A064655_cnt++] = v;
}

static void A064655_step(void)
{
    uint32_t x = A064655_last, best = UINT32_MAX;
    while (x > 1) {
        uint32_t p = A064655_spf[x];
        uint32_t c = A064655_nxt[p] ? A064655_nxt[p] : p;
        while (c <= A064655_LIM && A064655_used[c]) c += p;
        A064655_nxt[p] = c;
        if (c < best) best = c;
        while (x % p == 0) x /= p;
    }
    if (best > A064655_LIM) exit(1);
    A064655_used[best] = 1;
    A064655_last = best;
    if ((best & 1u) == 0) {
        A064655_cur++;
    } else if (A064655_cur > 0) {
        A064655_push(A064655_cur);
        A064655_cur = 0;
    }
}

seq_t a(idx_t n)
{
    if (!A064655_init) A064655_setup();
    if (n == 0) return (seq_t)0;
    while ((idx_t)A064655_cnt < n) A064655_step();
    return (seq_t)A064655_runs[(size_t)(n - 1)];
}
