/*
 * A209777  Number of q in the range 0<=q<=n for which A006002(n)+A006002(q)
 *          is prime.
 * https://oeis.org/A209777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of q in [0..n] such that A006002(n)+A006002(q) is
 *            prime
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Cached A006002 values:  A006002(k) = k*(k+1)^2/2. */
static seq_t *A_vals = NULL;   /* array of A006002 values */
static size_t  A_cap  = 0;     /* allocated capacity */

/* Ensure that A_vals holds values up to index n (inclusive). */
static void ensure_A_vals(idx_t n)
{
    if ((size_t)n < A_cap)
        return;

    size_t new_cap = (A_cap == 0) ? 1 : A_cap;
    while ((size_t)n >= new_cap)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, A_vals, new_cap);
    if (!tmp) abort();            /* out of memory */

    for (size_t i = A_cap; i < new_cap; ++i)
    {
        seq_t i_s  = (seq_t)i;
        seq_t ip1  = i_s + (seq_t)1;          /* (k+1)   */
        seq_t val  = i_s * ip1 * ip1 / (seq_t)2;  /* k*(k+1)^2/2 */
        tmp[i] = val;
    }

    A_vals = tmp;
    A_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1; a(0) is undefined, return 0 as safety */
        return (seq_t)0;

    ensure_A_vals(n);

    seq_t count = 0;
    seq_t An    = A_vals[(size_t)n];

    for (idx_t q = 0; q <= n; ++q)
    {
        seq_t sum = An + A_vals[(size_t)q];
        if (is_prime_big(sum))
            ++count;
    }
    return count;
}
