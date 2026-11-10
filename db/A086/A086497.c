/*
 * A086497  Primes arising in A086496. a(n) = Sum {A086946(k), k = 1 to 2n}=
 *          2n-th partial sum of A086496.
 * https://oeis.org/A086497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: A086496 is built greedily as b(2k-1) = least unused
 *            positive integer, b(2k) = least unused m making b(1)+...+b(2k)
 *            prime; a(n) = the 2n-th partial sum of b
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint8_t *A086497_used = NULL;
static size_t A086497_cap = 0;

static void A086497_ensure(size_t idx)
{
    if (idx < A086497_cap)
        return;
    size_t ncap = A086497_cap ? A086497_cap : 1024;
    while (ncap <= idx)
        ncap *= 2;
    uint8_t *p = (uint8_t *)realloc(A086497_used, ncap);
    if (!p)
        abort();
    memset(p + A086497_cap, 0, ncap - A086497_cap);
    A086497_used = p;
    A086497_cap = ncap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (A086497_used)
        memset(A086497_used, 0, A086497_cap);
    A086497_ensure((size_t)(4 * n + 64));

    uint64_t sum = 0;
    uint64_t smallest = 1; /* smallest possibly-unused number */

    for (idx_t k = 1; k <= n; ++k) {
        /* odd position: least unused number */
        A086497_ensure((size_t)smallest + 1);
        while (A086497_used[smallest]) {
            ++smallest;
            A086497_ensure((size_t)smallest + 1);
        }
        A086497_used[smallest] = 1;
        sum += smallest;

        /* even position: least unused m with sum + m prime */
        uint64_t m = 1;
        for (;;) {
            A086497_ensure((size_t)m + 1);
            if (!A086497_used[m] && is_prime_u64(sum + m))
                break;
            ++m;
        }
        A086497_used[m] = 1;
        sum += m;
    }
    return (seq_t)sum;
}
