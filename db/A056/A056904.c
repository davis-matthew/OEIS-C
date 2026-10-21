/*
 * A056904  Floor[p/24] where p is a prime which is 4 more than a square.
 * https://oeis.org/A056904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊((k_n)^2 + 4)/24⌋, where k_n is the nth positive integer
 *            (starting at 1) for which k_n^2 + 4 is prime
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"

static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? (*cap << 1) : 1024;
    if (newcap < needed)
        newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* out of memory */
    memset(tmp + *cap, 0, (newcap - *cap) * sizeof(seq_t));
    *arr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *vals = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next_k = 1;        /* next integer k to test */

    if (n < len)
        return vals[n];

    ensure_cap(&vals, &cap, n + 1);

    while (len <= n) {
        /* Find next k such that k^2 + 4 is prime */
        while (true) {
            uint64_t cand = (uint64_t)next_k * (uint64_t)next_k + 4;
            if (is_prime(cand))
                break;
            ++next_k;
        }
        uint64_t p = (uint64_t)next_k * (uint64_t)next_k + 4;
        seq_t val = (seq_t)(p / 24);     /* floor(p/24) */
        vals[len++] = val;
        ++next_k;
    }
    return vals[n];
}
