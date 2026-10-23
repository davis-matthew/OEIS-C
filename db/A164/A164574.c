/*
 * A164574  Numbers k such that k and k+6 are both prime powers.
 * https://oeis.org/A164574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164574: numbers k such that k and k+6 are both prime powers.
 * This implementation searches for such k incrementally and caches the
 * results so that a(n) can be returned for any n ≥ 1 in an efficient
 * O(n) time for n up to about 10 000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* ------------------------------------------------------------------ */
/*  Helper: compute floor( x^(1/e) ) for uint64_t values.             */
/* ------------------------------------------------------------------ */
/* Return true if base^exp ≤ limit, without overflow. */
static bool pow_leq(uint64_t base, int exp, uint64_t limit)
{
    uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        if (result > limit / base) return false;
        result *= base;
    }
    return result <= limit;
}

/* Return floor( x^(1/e) ).  Assume x > 0, e ≥ 2. */
static uint64_t nth_root(uint64_t x, int e)
{
    uint64_t low = 2, high = x;
    while (low < high) {
        uint64_t mid = (low + high + 1) / 2;
        if (pow_leq(mid, e, x))
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Test whether a uint64_t number is a prime power.                  */
/* ------------------------------------------------------------------ */
static bool is_prime_power_uint64(uint64_t x)
{
    if (x == 1) return true;                 /* by convention for this seq. */
    if (is_prime(x)) return true;             /* exponent 1 */

    /* upper bound for exponent (highest e such that 2^e <= x) */
    int bits = 64 - __builtin_clzll(x);       /* position of highest set bit */
    if (bits < 2) return false;               /* x < 4, already handled */

    for (int e = 2; e <= bits; ++e) {
        uint64_t root = nth_root(x, e);
        if (root < 2) continue;              /* root too small */
        /* verify root^e == x */
        if (!pow_leq(root, e, x)) continue;  /* should not happen */
        /* Recompute to check exact equality without overflow */
        uint64_t p = 1;
        for (int i = 0; i < e; ++i) p *= root;
        if (p != x) continue;
        if (is_prime(root)) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main routine: returns the nth term of the sequence.                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* dynamic array of found terms */
    static size_t sz  = 0;         /* number of terms stored */
    static size_t cap = 0;         /* array capacity */
    static uint64_t next = 1;      /* next integer to test */

    while (sz < n) {
        /* test current value and its +6 counterpart */
        if (is_prime_power_uint64(next) &&
            is_prime_power_uint64(next + 6)) {
            if (sz == cap) {                    /* grow array */
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – abort gracefully */
                    return 0;
                }
                seq = tmp;
                cap = new_cap;
            }
            seq[sz++] = (seq_t)next;
        }
        ++next;
    }
    return seq[sz - 1];
}
