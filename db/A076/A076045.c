/*
 * A076045  Primes that are three term sums of A076990.
 * https://oeis.org/A076045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n)+b(n+1)+b(n+2), where b = A076990: b(1)=1, b(2)=2,
 *            and b(n) is the smallest unused positive integer making
 *            b(n-2)+b(n-1)+b(n) prime (greedy search)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static bool A076045_isPrimeGeneric(seq_t n)
{
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    for (seq_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *A076990_seq = NULL;
static size_t A076990_seq_cap = 0;
static idx_t A076990_max = 0;
static bool *A076990_used = NULL;
static size_t A076990_used_cap = 0;

static void A076990_ensure_seq(idx_t n)
{
    if ((size_t)n <= A076990_seq_cap) return;
    size_t new_cap = A076990_seq_cap ? A076990_seq_cap << 1 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A076990_seq, new_cap);
    if (!tmp) abort();
    A076990_seq = tmp;
    A076990_seq_cap = new_cap;
}

static void A076990_ensure_used(idx_t need)
{
    if ((size_t)need < A076990_used_cap) return;
    size_t new_cap = A076990_used_cap ? A076990_used_cap << 1 : 32;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(A076990_used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + A076990_used_cap, 0, (new_cap - A076990_used_cap) * sizeof(bool));
    A076990_used = tmp;
    A076990_used_cap = new_cap;
}

static void A076990_extend(idx_t n)
{
    if (n <= A076990_max) return;
    A076990_ensure_seq(n);
    for (idx_t i = A076990_max + 1; i <= n; ++i) {
        if (i == 1) {
            A076990_seq[0] = (seq_t)1;
            A076990_ensure_used(1);
            A076990_used[1] = true;
            continue;
        }
        if (i == 2) {
            A076990_seq[1] = (seq_t)2;
            A076990_ensure_used(2);
            A076990_used[2] = true;
            continue;
        }
        seq_t prev2 = A076990_seq[i - 3];
        seq_t prev1 = A076990_seq[i - 2];
        idx_t candidate = 1;
        for (;;) {
            A076990_ensure_used(candidate);
            if (!A076990_used[(size_t)candidate]) {
                seq_t sum = prev2 + prev1 + (seq_t)candidate;
                if (A076045_isPrimeGeneric(sum)) break;
            }
            ++candidate;
        }
        A076990_seq[i - 1] = (seq_t)candidate;
        A076990_used[(size_t)candidate] = true;
    }
    A076990_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A076990_extend(n + 2);
    return A076990_seq[n - 1] + A076990_seq[n] + A076990_seq[n + 1];
}
