/*
 * A215099  a(0)=0, a(1)=1, a(n) = least k>a(n-1) such that k+a(n-2) is prime.
 * https://oeis.org/A215099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 4995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9523 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0,
   a(1) = 1,
   a(n) = least k > a(n-1) such that k + a(n-2) is prime
   (search starts at k = a(n-1) + 1). */

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

/* cached sequence */
static seq_t *seq = NULL;      /* array of computed values */
static size_t seq_len = 0;     /* number of values stored (0 for none yet) */
static size_t seq_cap = 0;     /* allocated capacity of seq */

/* Ensure that seq has capacity for indices up to 'needed' (inclusive). */
static void ensure_capacity(idx_t needed)
{
    /* allocate at least needed+1 slots */
    if (seq_cap > (size_t)needed) return;   /* enough already */
    size_t new_cap = (size_t)needed + 1;
    seq_t *new_seq = mem_resize(seq_t, seq, new_cap);
    seq = new_seq;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* fast return for already computed values */
    if ((size_t)n < seq_len) return seq[n];

    /* make sure we can store up to index n */
    ensure_capacity(n);

    /* compute missing terms from seq_len to n inclusive */
    for (idx_t i = seq_len; i <= n; ++i)
    {
        if (i == 0) {
            seq[0] = 0;    /* base case */
        } else if (i == 1) {
            seq[1] = 1;    /* base case */
        } else {
            seq_t a_nm1 = seq[i-1];
            seq_t a_nm2 = seq[i-2];
            /* start searching from the smallest integer > a(i-1) */
            seq_t k = a_nm1 + 1;
            seq_t sum = k + a_nm2;

            /* find smallest k with k + a(i-2) prime */
            while (!is_prime_big(sum))
            {
                k++;
                sum++;   /* k + a_nm2 incremented by 1 each step */
            }
            seq[i] = k;
        }
        seq_len = (size_t)i + 1;   /* one more term stored */
    }
    return seq[(size_t)n];
}
