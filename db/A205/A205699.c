/*
 * A205699  The number j such that 7 divides prime(k)-prime(j), where
 *          k(n)=A205698(n).
 * https://oeis.org/A205699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j, the (n - S(k-1))-th smallest j<k with prime(j) ≡
 *            prime(k) (mod 7), where k = A205698(n) and S(m) = sum_{t<=m}
 *            #{j<t : prime(j) ≡ prime(t) (mod 7)}; computed by sequential
 *            prime generation
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A205699_res = NULL;   /* residue mod 7 of prime(i), i>=1 at index i-1 */
static uint64_t *A205699_cum = NULL;        /* cum[i-1] = S(i) */
static size_t A205699_len = 0, A205699_cap = 0;
static uint64_t A205699_last = 0;
static uint64_t A205699_cnt[7] = {0,0,0,0,0,0,0};

static bool A205699_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static void A205699_gen(void)
{
    uint64_t c;
    if (A205699_last == 0) c = 2;
    else {
        c = A205699_last + 1;
        if (c > 2 && (c & 1) == 0) c++;
        while (!A205699_isp(c)) c += 2;
    }
    if (A205699_len == A205699_cap) {
        size_t nc = A205699_cap ? A205699_cap * 2 : 64;
        A205699_res = (unsigned char *)realloc(A205699_res, nc);
        A205699_cum = (uint64_t *)realloc(A205699_cum, nc * sizeof(uint64_t));
        if (!A205699_res || !A205699_cum) abort();
        A205699_cap = nc;
    }
    unsigned r = (unsigned)(c % 7);
    uint64_t prev = A205699_len ? A205699_cum[A205699_len - 1] : 0;
    A205699_cum[A205699_len] = prev + A205699_cnt[r];
    A205699_res[A205699_len] = (unsigned char)r;
    A205699_cnt[r]++;
    A205699_len++;
    A205699_last = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A205699_len == 0 || A205699_cum[A205699_len - 1] < (uint64_t)n)
        A205699_gen();
    /* binary search smallest index k (1-based) with cum >= n */
    size_t lo = 1, hi = A205699_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A205699_cum[mid - 1] >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    size_t k = lo;
    uint64_t before = (k >= 2) ? A205699_cum[k - 2] : 0;
    uint64_t want = (uint64_t)n - before;
    unsigned r = A205699_res[k - 1];
    uint64_t seen = 0;
    for (size_t j = 1; j < k; j++) {
        if (A205699_res[j - 1] == r) {
            seen++;
            if (seen == want) return (seq_t)j;
        }
    }
    return 0;
}
