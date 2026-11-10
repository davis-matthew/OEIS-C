/*
 * A050015  Numbers k such that b(k) > b(k+1), where b=A050012.
 * https://oeis.org/A050015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with b(k) > b(k+1), where b = A050012: * b(1)
 *            = 1; b(k) = floor(b(k-1)/2) if that is positive and not already
 *            in b, * otherwise b(k) = 7*b(k-1). Computed by generating b and
 *            scanning for descents
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Terms of A050012, kept as native 64-bit values (they stay far below 2^64
 * for the range needed here), so the comparisons are exact in every build. */
static uint64_t *A050015_b = NULL;
static size_t A050015_blen = 0, A050015_bcap = 0;

/* Indices k with b(k) > b(k+1), in increasing order. */
static uint64_t *A050015_desc = NULL;
static size_t A050015_dlen = 0, A050015_dcap = 0;

static bool A050015_seen(uint64_t v)
{
    size_t i;
    for (i = 0; i < A050015_blen; ++i)
        if (A050015_b[i] == v)
            return true;
    return false;
}

static void A050015_push_b(uint64_t v)
{
    if (A050015_blen == A050015_bcap) {
        size_t nc = A050015_bcap ? A050015_bcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A050015_b, nc * sizeof(uint64_t));
        if (!t) abort();
        A050015_b = t;
        A050015_bcap = nc;
    }
    A050015_b[A050015_blen++] = v;
}

static void A050015_push_desc(uint64_t k)
{
    if (A050015_dlen == A050015_dcap) {
        size_t nc = A050015_dcap ? A050015_dcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A050015_desc, nc * sizeof(uint64_t));
        if (!t) abort();
        A050015_desc = t;
        A050015_dcap = nc;
    }
    A050015_desc[A050015_dlen++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (A050015_blen == 0)
        A050015_push_b(1);               /* b(1) = 1 */

    while ((idx_t)A050015_dlen < n) {
        uint64_t prev = A050015_b[A050015_blen - 1];
        uint64_t cand = prev / 2;
        uint64_t next;
        if (cand > 0 && !A050015_seen(cand))
            next = cand;
        else
            next = prev * 7;
        A050015_push_b(next);
        /* b(k) is A050015_b[k-1]; we just appended b(blen) */
        if (prev > next)
            A050015_push_desc((uint64_t)(A050015_blen - 1));
    }

    return (seq_t)A050015_desc[n - 1];
}
