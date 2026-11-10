/*
 * A229312  Numbers n such that A031971(47058*n) == n (mod 47058*n).
 * https://oeis.org/A229312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A031971(47058*m) == m (mod
 *            47058*m), where A031971(N) = Sum_{k=1..N} k^N; found by a search
 *            that tests each m via CRT over the prime powers of N
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A229312_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    unsigned __int128 r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

static bool A229312_test(uint64_t n)
{
    uint64_t N = 47058ULL * n;
    uint64_t ps[40], es[40];
    int cnt = 0;
    static const uint64_t base[5] = {2, 3, 11, 23, 31};
    uint64_t m = n;
    for (int i = 0; i < 5; i++) {
        uint64_t p = base[i], e = 1;
        while (m % p == 0) { m /= p; e++; }
        ps[cnt] = p; es[cnt] = e; cnt++;
    }
    for (uint64_t p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            bool found = false;
            for (int i = 0; i < cnt; i++)
                if (ps[i] == p) { es[i] += e; found = true; }
            if (!found) { ps[cnt] = p; es[cnt] = e; cnt++; }
        }
    }
    if (m > 1) {
        bool found = false;
        for (int i = 0; i < cnt; i++)
            if (ps[i] == m) { es[i] += 1; found = true; }
        if (!found) { ps[cnt] = m; es[cnt] = 1; cnt++; }
    }
    /* pass 1: exponent 1 primes */
    for (int i = 0; i < cnt; i++) {
        if (es[i] != 1) continue;
        uint64_t p = ps[i];
        uint64_t T = (N % (p - 1) == 0) ? p - 1 : 0;
        uint64_t lhs = (uint64_t)((((unsigned __int128)((N / p) % p)) * T) % p);
        if (lhs != n % p) return false;
    }
    /* pass 2: higher prime powers, direct */
    for (int i = 0; i < cnt; i++) {
        if (es[i] == 1) continue;
        uint64_t q = 1;
        for (uint64_t j = 0; j < es[i]; j++) q *= ps[i];
        uint64_t T = 0;
        for (uint64_t k = 1; k < q; k++) {
            T += A229312_powmod(k, N, q);
            if (T >= q) T -= q;
        }
        uint64_t lhs = (uint64_t)((((unsigned __int128)((N / q) % q)) * T) % q);
        if (lhs != n % q) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t count = 0, cap = 0;
    static uint64_t next = 1;
    while (count < (size_t)n) {
        if (A229312_test(next)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[count++] = next;
        }
        next++;
    }
    return (seq_t)terms[n - 1];
}
