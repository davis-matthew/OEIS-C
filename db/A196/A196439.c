/*
 * A196439  a(n) = the sum of numbers k <= n such that GCQ_A(n, k) = LCQ_A(n,
 *          k) = 0 (see definition in comments).
 * https://oeis.org/A196439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of k <= n such that the greatest (LCM) and least
 *            common (DIVISORS) non‑divisors of n and k are zero. This is
 *            equivalent to: for every integer q (1 <= q <= min(n,k)), q
 *            divides n or q divides k. The implementation precomputes for
 *            each k the least common multiple (LCM) of all integers <=...
 * Range:     u8 n <= 9849, u16 n <= 9849, u32 n <= 9849, u64 n <= 9849,
 *            u128 n <= 9849, big any n
 * Helpers:   _gcd_uint64, _lcm_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_gcd_uint64.h"
#include "../utils/_lcm_cap.h"

static size_t _max_k = 0;
static uint64_t *_L = NULL;

/* Euclidean GCD for uint64_t */

/* Ensure that array _L is computed up to at least n (max of current). */
static void _ensure_up_to(idx_t n)
{
    if (n <= _max_k) return;
    size_t new_max = (size_t)n;
    uint64_t *new_L = (uint64_t*)malloc((new_max + 1) * sizeof(uint64_t));
    if (!new_L) return; /* silently handle memory failure */
    if (_L) {
        memcpy(new_L, _L, (_max_k + 1) * sizeof(uint64_t));
        free(_L);
    }
    const uint64_t CAP = 10001;
    for (size_t k = _max_k + 1; k <= new_max; ++k) {
        uint64_t l = 1;
        for (size_t q = 1; q <= k; ++q) {
            if (k % q == 0) continue;      /* q divides k -> skip */
            l = _lcm_cap(l, q, CAP);
            if (l > CAP) break;            /* no need to continue */
        }
        if (l > CAP) l = CAP + 1;          /* mark as unusable */
        new_L[k] = l;
    }
    _L = new_L;
    _max_k = new_max;
}

seq_t a(idx_t n)
{
    /* n is at least 1 */
    _ensure_up_to(n);

    seq_t sum = 0;
    for (size_t k = 1; k <= n; ++k) {
        uint64_t lk = _L[k];
        if (lk == 0) continue;            /* should not happen */
        if (lk <= (uint64_t)n && ((uint64_t)n % lk) == 0) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
