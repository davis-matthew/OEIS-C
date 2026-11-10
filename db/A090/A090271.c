/*
 * A090271  Palindromes in A090272.
 * https://oeis.org/A090271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- a(1) = 11; then for each palindromic k == 1,3,7,9 (mod
 *            10) in increasing order, take the least r >= 1 with
 *            concat(k,r,k) prime (as in A090270), and keep that prime when it
 *            is itself a palindrome
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   is_prime_u128, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u128.h"
#include "../utils/mem.h"

typedef unsigned __int128 A090271_u128;

static bool A090271_pal(A090271_u128 v)
{
    A090271_u128 rev = 0, t = v;
    while (t) { rev = rev * 10 + (unsigned)(t % 10); t /= 10; }
    return rev == v;
}

static int A090271_len(A090271_u128 v)
{
    int d = 0;
    do { ++d; v /= 10; } while (v);
    return d;
}

static A090271_u128 A090271_p10(int e)
{
    A090271_u128 v = 1;
    while (e-- > 0) v *= 10;
    return v;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size = 0, cnt = 0;
    static uint64_t next_k = 3;

    if (n == 0) return (seq_t)0;
    if (!cache) {
        size = 16;
        cache = mem_new(seq_t, size);
        if (!cache) return (seq_t)0;
        cache[cnt++] = (seq_t)11;
    }
    while (cnt < (size_t)n) {
        uint64_t k = next_k++;
        unsigned d = (unsigned)(k % 10);
        if (!(d == 1 || d == 3 || d == 7 || d == 9)) continue;
        if (!A090271_pal((A090271_u128)k)) continue;
        int lk = A090271_len((A090271_u128)k);
        A090271_u128 pk = A090271_p10(lk);
        A090271_u128 cand = 0;
        uint64_t r;
        for (r = 1; ; ++r) {
            int lr = A090271_len((A090271_u128)r);
            cand = (A090271_u128)k * A090271_p10(lk + lr)
                 + (A090271_u128)r * pk + (A090271_u128)k;
            if (is_prime_u128(cand)) break;
        }
        if (!A090271_pal(cand)) continue;
        if (cnt >= size) {
            size_t ns = size * 2;
            seq_t *tmp = mem_resize(seq_t, cache, ns);
            if (!tmp) return (seq_t)0;
            cache = tmp;
            size = ns;
        }
        {
            /* build value in seq_t from decimal digits to stay exact */
            seq_t v = 0;
            A090271_u128 p = A090271_p10(A090271_len(cand) - 1);
            A090271_u128 t = cand;
            while (p) {
                unsigned dig = (unsigned)(t / p);
                v = v * 10 + dig;
                t %= p;
                p /= 10;
            }
            cache[cnt++] = v;
        }
    }
    return cache[n - 1];
}
