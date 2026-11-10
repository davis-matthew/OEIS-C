/*
 * A225046  Position of the first occurrence of the n-th prime in A225017, or
 *          0 if prime(n) never occurs.
 * https://oeis.org/A225046
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>=0 with A225017(m) = prime(n), or 0 if none
 *            found; computed by a search (incrementally building 5^m, with a
 *            search cap)
 * Range:     u8 n <= 47, u16 n <= 240, u32 n <= 240, u64 n <= 240,
 *            u128 n <= 240, big any n
 * Validated: standard, pass, 77 terms through n = 233 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define A225046_LIMIT 50000
#define A225046_BLOCKS 3500
#define A225046_BASE 1000000000U

static uint32_t A225046_vals[A225046_LIMIT + 1];
static uint32_t A225046_digits[A225046_BLOCKS];
static uint16_t A225046_ds[1000];
static int A225046_size = 0;
static int A225046_cnt = 0;
static bool A225046_init = false;

static uint32_t A225046_reduce(uint64_t s)
{
    if (s == 0) return 0;
    while (s % 5 == 0) s /= 5;
    while (s % 2 == 0) s /= 2;
    return (uint32_t)s;
}

static uint64_t A225046_sum(void)
{
    uint64_t sum = 0;
    for (int j = 0; j < A225046_size; ++j) {
        uint32_t v = A225046_digits[j];
        sum += A225046_ds[v % 1000] + A225046_ds[(v / 1000) % 1000] + A225046_ds[v / 1000000];
    }
    return sum;
}

static void A225046_extend(int target)
{
    if (!A225046_init) {
        for (int i = 0; i < 1000; ++i)
            A225046_ds[i] = (uint16_t)(i % 10 + (i / 10) % 10 + i / 100);
        A225046_init = true;
    }
    if (A225046_cnt == 0) {
        A225046_digits[0] = 1;
        A225046_size = 1;
        A225046_vals[0] = A225046_reduce(1);
        A225046_cnt = 1;
    }
    while (A225046_cnt < target) {
        uint64_t carry = 0;
        for (int j = 0; j < A225046_size; ++j) {
            uint64_t prod = (uint64_t)A225046_digits[j] * 5 + carry;
            A225046_digits[j] = (uint32_t)(prod % A225046_BASE);
            carry = prod / A225046_BASE;
        }
        if (carry) A225046_digits[A225046_size++] = (uint32_t)carry;
        A225046_vals[A225046_cnt++] = A225046_reduce(A225046_sum());
    }
}

static uint64_t A225046_nth_prime(uint64_t n)
{
    uint64_t c = 0, p = 1;
    while (c < n) {
        ++p;
        bool pr = true;
        for (uint64_t d = 2; d * d <= p; ++d)
            if (p % d == 0) { pr = false; break; }
        if (pr) ++c;
    }
    return p;
}

seq_t a(idx_t n)
{
    uint64_t p = A225046_nth_prime((uint64_t)n);
    if (p == 2 || p == 5) return (seq_t)0;
    int pos = 0;
    for (;;) {
        for (; pos < A225046_cnt; ++pos)
            if ((uint64_t)A225046_vals[pos] == p) return (seq_t)pos;
        if (A225046_cnt >= A225046_LIMIT) return (seq_t)0;
        int target = A225046_cnt + 1000;
        if (target > A225046_LIMIT) target = A225046_LIMIT;
        A225046_extend(target);
    }
}
