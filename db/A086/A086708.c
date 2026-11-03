/*
 * A086708  Primes p such that p-1 and p+1 are both divisible by cubes (other
 *          than 1).
 * https://oeis.org/A086708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p with the property that both p-1 and p+1 are divisible
 *            by a non‑trivial cube
 * Range:     u8 none, u16 n <= 228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9624 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mem, mulmod, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* Returns true if n has a non‑trivial cube divisor (i.e. some prime factor occurs with exponent ≥ 3). */
static bool has_cube_divisor_uint64(uint64_t n)
{
    if (n <= 1) return false;
    while (n > 1) {
        uint64_t f = smallest_prime_factor(n);
        int exp = 0;
        while (n % f == 0) {
            n /= f;
            ++exp;
            if (exp >= 3) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1 */

    /* Static cache of found terms. */
    static seq_t *seq = NULL;
    static size_t sz   = 0;           /* number of terms stored */
    static size_t cap  = 0;           /* allocated capacity */
    static uint64_t last = 1;         /* last integer inspected (start from 1 so next_prime(1) = 2) */

    if (seq == NULL) {
        cap = 16;
        seq = alloc_seq_array(cap);
    }

    /* Generate terms until we have at least n of them. */
    while (sz < n) {
        uint64_t p = next_prime(last);
        last = p;

        if (has_cube_divisor_uint64(p - 1) && has_cube_divisor_uint64(p + 1)) {
            if (sz >= cap) {
                grow_seq((seq_t **)&seq, &cap, sz + 1);
            }
            seq[sz++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
