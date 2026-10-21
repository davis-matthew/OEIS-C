/*
 * A083481  Squarefree part of the n-th oblong number.
 * https://oeis.org/A083481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree part of n * squarefree part of n+1 *
 *            (equivalently, a(n) = A007913(n*(n+1)) =
 *            A007913(n)*A007913(n+1)) * for n ≥ 1. a(1)=2, a(2)=6, a(3)=3, …
 *            as in A083481
 * Range:     u8 n <= 20, u16 n <= 256, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------
 * Prime generation (once per process)
 * ------------------------------------------------------------------ */
#if 10000 < 32768
#define MAX_PRIME 10000
#else
#define MAX_PRIME 32768
#endif

static unsigned primes_tbl[MAX_PRIME + 1];
static size_t primes_cnt = 0;
static bool primes_initted = false;

static void init_primes(void) {
    if (primes_initted) return;
    bool is_composite[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_composite[i] = false;
    for (unsigned i = 2; i * i <= (unsigned)MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            for (unsigned j = i * i; j <= (unsigned)MAX_PRIME; j += i)
                is_composite[j] = true;
        }
    }
    for (unsigned i = 2; i <= (unsigned)MAX_PRIME; ++i)
        if (!is_composite[i])
            primes_tbl[primes_cnt++] = i;
    primes_initted = true;
}

/* ------------------------------------------------------------------
 * Squarefree part of a non‑negative integer <= 64‑bit.
 * ------------------------------------------------------------------ */
static seq_t squarefree_of(uint64_t v) {
    uint64_t x = v;
    seq_t res = 1;
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        unsigned p = primes_tbl[idx];
        if ((uint64_t)p * (uint64_t)p > x) break;
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (cnt & 1) res *= (seq_t)p;
    }
    if (x > 1)   /* remaining prime factor > sqrt(v) */
        res *= (seq_t)x;
    return res;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* n ≥ 1 by sequence definition; behaviour below 1 is irrelevant */
    init_primes();
    uint64_t nn = (uint64_t)n;
    seq_t sf1 = squarefree_of(nn);
    seq_t sf2 = squarefree_of(nn + 1);
    return sf1 * sf2;
}
