/*
 * A001130  Number of graphical basis partitions of 2n.
 * https://oeis.org/A001130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; a search. Count Frobenius symbols (a_1..a_k |
 *            b_1..b_k) of 2n where each step either decreases a by exactly 1
 *            or decreases b by exactly 1, the symbol ends when some a_k or
 *            b_k is 0, and the graphical condition sum_{j<=i} (b_j - a_j - 1)
 *            >= 0 holds for every i. Memoized recursion
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A1130_HBITS 22
#define A1130_HSIZE (1u << A1130_HBITS)
#define A1130_POOL (1u << 21)

static uint64_t *a1130_keys = 0;
static uint32_t *a1130_vals = 0;
static seq_t a1130_pool[A1130_POOL];
static uint32_t a1130_npool = 0;

static uint32_t a1130_slot(uint64_t key) {
    uint64_t h = key * 0x9E3779B97F4A7C15ULL;
    uint32_t i = (uint32_t)(h >> (64 - A1130_HBITS));
    while (a1130_keys[i] != 0 && a1130_keys[i] != key)
        i = (i + 1) & (A1130_HSIZE - 1);
    return i;
}

static seq_t a1130_rec(long long a, long long b, long long rem, long long s) {
    long long ap, bp, m, cap;
    uint64_t key = 0;
    uint32_t i;
    bool cacheable;
    seq_t cnt = 0;
    if (a == 0 || b == 0) {
        if (rem == 0) cnt = 1;
        return cnt;
    }
    m = a < b ? a : b;
    if (rem < m) return cnt;
    /* maximum weight still reachable from (a,b) */
    if (rem > m * (a + b + 1) - m * (m + 1)) return cnt;
    /* beyond this slack the graphical constraint can never bind again */
    cap = a * (a + 1) / 2 + a;
    if (a + rem < cap) cap = a + rem;
    if (s > cap) s = cap;

    cacheable = (a1130_keys != 0 && a < 65536 && b < 65536 && s < 65536 && rem < 65535);
    if (cacheable) {
        key = ((uint64_t)a | ((uint64_t)b << 16) | ((uint64_t)s << 32) | ((uint64_t)rem << 48)) + 1;
        i = a1130_slot(key);
        if (a1130_keys[i] == key) return a1130_pool[a1130_vals[i]];
    }

    /* option A: a' = a-1, b' < b */
    ap = a - 1;
    {
        long long lo = ap + 1 - s;
        if (lo < 0) lo = 0;
        for (bp = lo; bp < b; bp++) {
            long long w = ap + bp + 1;
            if (w > rem) break;
            cnt += a1130_rec(ap, bp, rem - w, s + bp - ap - 1);
        }
    }
    /* option B: b' = b-1, a' < a-1 */
    bp = b - 1;
    for (ap = 0; ap <= a - 2; ap++) {
        long long w = ap + bp + 1;
        long long ns = s + bp - ap - 1;
        if (w > rem || ns < 0) break;
        cnt += a1130_rec(ap, bp, rem - w, ns);
    }

    if (cacheable && a1130_npool < A1130_POOL) {
        i = a1130_slot(key);
        if (a1130_keys[i] == 0) {
            a1130_pool[a1130_npool] = cnt;
            a1130_keys[i] = key;
            a1130_vals[i] = a1130_npool;
            a1130_npool++;
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    long long N = (long long)n * 2;
    long long a1, b1;
    seq_t cnt = 0;
    if (a1130_keys == 0) {
        a1130_keys = (uint64_t *)calloc(A1130_HSIZE, sizeof(uint64_t));
        a1130_vals = (uint32_t *)calloc(A1130_HSIZE, sizeof(uint32_t));
        if (a1130_keys == 0 || a1130_vals == 0) {
            free(a1130_keys);
            free(a1130_vals);
            a1130_keys = 0;
            a1130_vals = 0;
        }
    }
    for (a1 = 0; 2 * a1 + 2 <= N; a1++) {
        for (b1 = a1 + 1; a1 + b1 + 1 <= N; b1++) {
            long long w = a1 + b1 + 1;
            cnt += a1130_rec(a1, b1, N - w, b1 - a1 - 1);
        }
    }
    return cnt;
}
