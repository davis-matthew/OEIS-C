/*
 * A139361  Primes A080478(n)^2 + A080478(n+1)^2.
 * https://oeis.org/A139361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080478(n)^2 + A080478(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A080478(1)=1; for n>1, A080478(n) is the smallest k>A080478(n-1) with
    A080478(n-1)^2 + k^2 prime.  This function returns the nth term
    of A139361 for any n ≥ 1.  It memoises all needed A080478 values
    and corresponding primes so that calls in any order are O(n) total.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Memoisation storage */
static seq_t *a_vals   = NULL;   /* A080478 values, a_vals[0] = 1         */
static size_t a_len    = 0;      /* number of stored A080478 terms        */
static size_t a_cap    = 0;      /* capacity of a_vals array              */
static seq_t *primes   = NULL;   /* stored prime sums a(n)                */
static size_t p_len    = 0;      /* number of stored primes              */
static size_t p_cap    = 0;      /* capacity of primes array              */

/* Grow an array of seq_t elements to at least 'needed' size.  */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the nth term of A139361.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* Initialise memoisation on first use.  */
    if (a_len == 0) {
        ensure_cap(&a_vals, &a_cap, 2);
        a_vals[0] = 1;             /* A080478(1) = 1 */
        a_len = 1;
    }

    /* Generate A080478 terms and prime sums up to index n.  */
    while (a_len <= (size_t)n) {
        seq_t prev = a_vals[a_len - 1];
        seq_t cand = prev + 1;
        seq_t sum;

        /* Find the smallest k>prev such that prev^2 + k^2 is prime.  */
        for (;;) {
            sum = prev * prev + cand * cand;
            if (is_prime_big(sum)) break;
            ++cand;
        }

        /* Store new A080478 term.  */
        ensure_cap(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = cand;
        ++a_len;

        /* Store its associated prime sum a(a_len-1).  */
        ensure_cap(&primes, &p_cap, p_len + 1);
        primes[p_len] = sum;
        ++p_len;
    }

    /* primes array is zero‑indexed; the nth term is primes[n-1].  */
    return primes[n - 1];
}
