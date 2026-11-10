/*
 * A086527  Primes arising in A055265.
 * https://oeis.org/A086527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055265(n) + A055265(n+1), where A055265(1) = 1 and
 *            A055265(k) is the smallest unused positive integer with
 *            A055265(k) + A055265(k-1) prime (greedy search)
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 849 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *A086527_s = NULL;
static size_t A086527_len = 0, A086527_cap = 0;
static unsigned char *A086527_used = NULL;
static size_t A086527_ucap = 0;
static size_t A086527_minun = 1;

static bool A086527_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m < 4) return true;
    if (m % 2 == 0) return false;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static void A086527_ensure_used(size_t need)
{
    if (need <= A086527_ucap) return;
    size_t nc = A086527_ucap ? A086527_ucap : 256;
    while (nc < need) nc *= 2;
    A086527_used = (unsigned char *)realloc(A086527_used, nc);
    memset(A086527_used + A086527_ucap, 0, nc - A086527_ucap);
    A086527_ucap = nc;
}

static void A086527_extend(size_t upto)
{
    if (A086527_len == 0) {
        A086527_cap = 256;
        A086527_s = (uint64_t *)malloc(A086527_cap * sizeof(uint64_t));
        A086527_ensure_used(256);
        A086527_s[0] = 1;
        A086527_used[1] = 1;
        A086527_len = 1;
        A086527_minun = 2;
    }
    while (A086527_len < upto) {
        if (A086527_len + 1 > A086527_cap) {
            A086527_cap *= 2;
            A086527_s = (uint64_t *)realloc(A086527_s, A086527_cap * sizeof(uint64_t));
        }
        uint64_t prev = A086527_s[A086527_len - 1];
        size_t c = A086527_minun;
        for (;;) {
            A086527_ensure_used(c + 1);
            if (!A086527_used[c] && A086527_isprime(prev + (uint64_t)c)) break;
            c++;
        }
        A086527_s[A086527_len++] = (uint64_t)c;
        A086527_used[c] = 1;
        while (A086527_used[A086527_minun]) {
            A086527_minun++;
            A086527_ensure_used(A086527_minun + 1);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t k = (size_t)n;
    A086527_extend(k + 1);
    return (seq_t)(A086527_s[k - 1] + A086527_s[k]);
}
