/*
 * A076920  Highest common factor of a pair of successive terms of A076919.
 * https://oeis.org/A076920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(b(n), b(n+1)) where b = A076919: b(1)=1, b(2)=2,
 *            b(k+1) = least m > b(k) with gcd(b(k),m) > 1 and gcd(b(k),m) !=
 *            gcd(b(k-1),b(k)); computed by search
 * Range:     u8 n <= 29, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *A076920_b = NULL;
static idx_t A076920_len = 0;
static idx_t A076920_cap = 0;

static void A076920_push(seq_t v)
{
    if (A076920_len >= A076920_cap) {
        idx_t nc = A076920_cap ? A076920_cap * 2 : 256;
        if (A076920_b == NULL) {
            A076920_b = mem_new(seq_t, nc);
        } else {
            A076920_b = mem_resize(seq_t, A076920_b, nc);
        }
        A076920_cap = nc;
    }
    A076920_b[A076920_len++] = v;
}

static void A076920_ensure(idx_t cnt)
{
    if (A076920_len == 0) {
        A076920_push((seq_t)1);
        A076920_push((seq_t)2);
    }
    while (A076920_len < cnt) {
        seq_t prev = A076920_b[A076920_len - 1];
        seq_t pg = gcd(A076920_b[A076920_len - 2], prev);
        seq_t cand = prev + 1;
        for (;;) {
            seq_t g = gcd(prev, cand);
            if (g > 1 && g != pg) break;
            cand++;
        }
        A076920_push(cand);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A076920_ensure(n + 1);
    return gcd(A076920_b[n - 1], A076920_b[n]);
}
