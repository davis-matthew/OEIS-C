/*
 * A086939  Greatest m with A086571(m) = n.
 * https://oeis.org/A086939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = position of the second occurrence of n in A086571, where
 *            A086571(1)=1 and A086571(m) = smallest divisor of m that
 *            occurred at most once among earlier terms; computed by
 *            simulation
 * Range:     u8 n <= 47, u16 n <= 5999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9922 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A086939_freq = NULL;
static uint64_t *A086939_second = NULL;
static uint64_t A086939_cap = 0;
static uint64_t A086939_cur = 0;

static void A086939_ensure(uint64_t need)
{
    if (need < A086939_cap) return;
    uint64_t nc = A086939_cap ? A086939_cap : 1024;
    while (nc <= need) nc *= 2;
    uint32_t *f = (uint32_t *)realloc(A086939_freq, nc * sizeof(uint32_t));
    if (!f) abort();
    A086939_freq = f;
    uint64_t *s = (uint64_t *)realloc(A086939_second, nc * sizeof(uint64_t));
    if (!s) abort();
    A086939_second = s;
    for (uint64_t i = A086939_cap; i < nc; ++i) {
        A086939_freq[i] = 0;
        A086939_second[i] = 0;
    }
    A086939_cap = nc;
}

static void A086939_step(void)
{
    uint64_t cur = ++A086939_cur;
    A086939_ensure(cur + 1);
    uint64_t val = cur;
    if (cur == 1) {
        val = 1;
    } else {
        bool found = false;
        uint64_t d;
        for (d = 1; d * d <= cur; ++d) {
            if (cur % d == 0 && A086939_freq[d] <= 1) {
                val = d;
                found = true;
                break;
            }
        }
        if (!found) {
            uint64_t r = d - 1;
            if (r * r > cur) --r;
            for (d = r; d >= 1; --d) {
                if (cur % d == 0 && A086939_freq[cur / d] <= 1) {
                    val = cur / d;
                    break;
                }
            }
        }
    }
    A086939_freq[val] += 1;
    if (A086939_freq[val] == 2) A086939_second[val] = cur;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A086939_ensure((uint64_t)n + 1);
    while (A086939_second[(uint64_t)n] == 0) {
        A086939_step();
    }
    return (seq_t)A086939_second[(uint64_t)n];
}
