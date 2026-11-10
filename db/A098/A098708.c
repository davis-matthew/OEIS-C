/*
 * A098708  a(n) = gcd(A098707(n), A098707(n+1)).
 * https://oeis.org/A098708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = gcd(A098707(n), A098707(n+1)); A098707 computed by search
 *            (a(1)=2, next = smallest m > prev with gcd(m,prev)=d, 1<d<10,
 *            digit d occurring exactly once in both)
 * Range:     u8 n <= 40, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A098708_digit_once(seq_t x, int d)
{
    int count = 0;
    while (x) {
        int dig = (int)(x % 10);
        if (dig == d) count++;
        x /= 10;
    }
    return count == 1;
}

static seq_t A098708_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static size_t A098708_alloc = 0;
static size_t A098708_count = 0;
static seq_t *A098708_cache = NULL;

static void A098708_ensure(size_t need)
{
    if (need <= A098708_alloc) return;
    size_t new_alloc = A098708_alloc ? A098708_alloc : 16;
    while (new_alloc < need) new_alloc <<= 1;
    seq_t *nc = mem_resize(seq_t, A098708_cache, new_alloc);
    if (nc) {
        A098708_cache = nc;
        A098708_alloc = new_alloc;
    }
}

static seq_t A098707_a(idx_t n)
{
    if (n == 0) return 0;
    if (A098708_count == 0) {
        A098708_ensure(16);
        A098708_cache[0] = (seq_t)2;
        A098708_count = 1;
    }
    while (A098708_count < (size_t)n) {
        seq_t prev = A098708_cache[A098708_count - 1];
        seq_t m = prev + (seq_t)1;
        for (;;) {
            seq_t g = A098708_gcd(m, prev);
            if (g > (seq_t)1 && g < (seq_t)10) {
                int d = (int)g;
                if (A098708_digit_once(prev, d) && A098708_digit_once(m, d))
                    break;
            }
            m = m + (seq_t)1;
        }
        A098708_ensure(A098708_count + 1);
        A098708_cache[A098708_count] = m;
        ++A098708_count;
    }
    return A098708_cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    seq_t x = A098707_a(n);
    seq_t y = A098707_a(n + 1);
    return A098708_gcd(x, y);
}
