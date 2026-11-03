/*
 * A175600  Primes of form 4k+1 where k is a Pythagorean prime.
 * https://oeis.org/A175600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p = 4*k + 1 and k itself is a prime of the
 *            form 4*m + 1 (A175600). The routine searches for such primes on
 *            demand and memoizes them
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq_arr = NULL;   /* Memoized sequence values   */
static size_t seq_len = 0;      /* Number of terms stored      */
static size_t seq_cap = 0;      /* Allocated capacity          */
static seq_t next_candidate = 5;/* Next 4*k + 1 to test        */

/* Ensure the storage can hold at least 'needed' terms. */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = mem_resize(seq_t, seq_arr, new_cap);
    seq_cap = new_cap;
}

/* Append a new term to the memoized sequence. */
static void add_term(seq_t val)
{
    if (seq_len + 1 > seq_cap) grow_seq(seq_len + 1);
    seq_arr[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* Undefined for non‑positive n */
    while (seq_len < n)
    {
        seq_t p = next_candidate;  /* Candidate prime 4*k + 1   */
        next_candidate += 4;       /* Advance to next odd 4*k+1 */

        if (!is_prime_big(p))      /* p must be prime            */
            continue;

        seq_t k = (p - 1) / 4;     /* k = (p-1)/4                */
        if ((k & 3) != 1)          /* k must be ≡ 1 (mod 4)      */
            continue;

        if (!is_prime_big(k))      /* k itself must be prime     */
            continue;

        add_term(p);               /* Found a valid term         */
    }
    return seq_arr[n - 1];
}
