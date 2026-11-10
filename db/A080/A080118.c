/*
 * A080118  Intersection of A080117 and A014486.
 * https://oeis.org/A080118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term, in increasing order, of the Dyck words
 *            (A014486) of length p-1 for odd primes p, built as follows. The
 *            first half is the Legendre indicator bits [(i/p)=1] for
 *            i=1..(p-1)/2. The second half is the reverse complement of the
 *            first half. A prime p gives a term only if the resulting...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *A080118_found = NULL;
static size_t A080118_cnt = 0, A080118_cap = 0;
static uint64_t A080118_next = 3;

static bool A080118_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* test whether prime p gives a Dyck word; fills qr (size p) */
static bool A080118_test(uint64_t p)
{
    uint64_t half = (p - 1) / 2, x;
    unsigned char *qr = (unsigned char *)calloc((size_t)p, 1);
    if (!qr) return false;
    for (x = 1; x <= half; x++) qr[(x * x) % p] = 1;
    long long bal = 0;
    bool ok = true;
    for (x = 1; x <= half; x++) {
        bal += qr[x] ? 1 : -1;
        if (bal < 0) { ok = false; break; }
    }
    free(qr);
    return ok;
}

static seq_t A080118_build(uint64_t p)
{
    uint64_t half = (p - 1) / 2, x;
    unsigned char *qr = (unsigned char *)calloc((size_t)p, 1);
    seq_t v = 0;
    if (!qr) return v;
    for (x = 1; x <= half; x++) qr[(x * x) % p] = 1;
    for (x = 1; x <= half; x++) {
        v = v << 1;
        if (qr[x]) v = v | 1;
    }
    for (x = half; x >= 1; x--) {
        v = v << 1;
        if (!qr[x]) v = v | 1;
    }
    free(qr);
    return v;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while ((idx_t)A080118_cnt < n) {
        uint64_t p = A080118_next;
        A080118_next += 2;
        if (!A080118_isprime(p)) continue;
        if (!A080118_test(p)) continue;
        if (A080118_cnt == A080118_cap) {
            size_t nc = A080118_cap ? A080118_cap * 2 : 32;
            uint64_t *np = (uint64_t *)realloc(A080118_found, nc * sizeof(uint64_t));
            if (!np) return (seq_t)0;
            A080118_found = np;
            A080118_cap = nc;
        }
        A080118_found[A080118_cnt++] = p;
    }
    return A080118_build(A080118_found[n - 1]);
}
