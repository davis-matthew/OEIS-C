/*
 * A051144  Nonsquarefree nonsquares: each term has a square factor but is not
 *          a perfect square itself.
 * https://oeis.org/A051144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9167 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A051144:
   a(n) = the n‑th positive integer that has at least one prime
          squared factor but is not a perfect square itself
          (i.e., not squarefree and not a square). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool good_uint64(uint64_t v)
{
    bool has_gt1 = false;   /* at least one exponent > 1  */
    bool has_odd = false;   /* at least one exponent odd   */
    uint64_t n = v;

    for (uint64_t p = 2; p * p <= n; ++p) {
        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            ++cnt;
            if (cnt >= 2)
                has_gt1 = true;
        }
        if (cnt > 0 && (cnt & 1))
            has_odd = true;
    }
    if (n > 1) {   /* remaining prime factor with exponent 1 */
        has_odd = true;
    }
    return has_gt1 && has_odd;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; behaviour below 1 is undefined */

    static seq_t *seq = NULL;
    static size_t len = 0;   /* number of terms already stored      */
    static size_t cap = 0;   /* capacity of allocated array         */
    static uint64_t cur = 1; /* last tested integer (start at 1)    */

    /* generate terms until we have at least n of them */
    while (len < n) {
        do {
            ++cur;
        } while (!good_uint64(cur));

        /* grow array if needed */
        if (len == cap) {
            size_t newcap = cap ? cap << 1 : 16;
            seq_t *tmp = mem_resize(seq_t, seq, newcap);
            if (!tmp)
                exit(EXIT_FAILURE);   /* allocation failed */
            seq = tmp;
            cap = newcap;
        }
        seq[len++] = (seq_t)cur;
    }
    return seq[n - 1];
}
