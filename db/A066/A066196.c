/*
 * A066196  Primes which have an equal number of zeros and ones in their
 *          binary expansion.
 * https://oeis.org/A066196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime whose binary representation contains
 *            exactly as many 0‑bits as 1‑bits (OEIS A066196). The function
 *            searches primes in increasing order, tests them for primality,
 *            counts the bits, and collects those that satisfy the balance
 *            condition
 * Range:     u8 n <= 7, u16 n <= 772, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_u64, modmul128, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bitlen_u64.h"
#include "../utils/modmul128.h"
#include "../utils/popcount_u64.h"

static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = modmul128(r, a, mod);
        a = modmul128(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL,
        28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = modmul128(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -------- Bit utilities for uint64_t -------- */

/* -------- Storage for collected primes -------- */
static seq_t *buf = NULL;
static size_t buf_len = 0;
static size_t buf_cap = 0;

static void append_balanced(seq_t val)
{
    if (buf_len == buf_cap) {
        size_t newcap = buf_cap ? buf_cap * 2 : 16;
        seq_t *newbuf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!newbuf) abort();
        buf = newbuf;
        buf_cap = newcap;
    }
    buf[buf_len++] = val;
}

/* -------- Main sequence function -------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the problem, but safe */

    static uint64_t cur = 2;      /* next prime candidate to test */
    static idx_t balanced_found = 0; /* number of balanced primes found so far */

    /* Ensure buffer initialized lazily on first use */
    if (buf_len == 0 && buf_cap == 0) { buf = NULL; buf_cap = 0; }

    while (balanced_found < n) {
        if (isPrime_u64(cur)) {
            int bl = bitlen_u64(cur);
            if ((bl & 1) == 0) { /* even number of bits */
                int ones = popcount_u64(cur);
                if (ones * 2 == bl) {
                    append_balanced((seq_t)cur);
                    ++balanced_found;
                    /* found a balanced prime, continue to next candidate */
                }
            }
        }
        /* advance to next candidate, skipping even numbers */
        if (cur == 2) cur = 3;
        else cur += 2;
    }

    return buf[n - 1];
}
