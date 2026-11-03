/*
 * A165666  Primes p such that (p^2-2)/7 is not prime.
 * https://oeis.org/A165666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 958 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165666: primes p such that (p^2-2)/7 is not prime */

/* The function a(n) returns the n‑th prime p satisfying the condition.
   It builds the sequence incrementally and stores the terms in a static
   array.  The array capacity is set large enough to cover the required
   range (≈10000 terms).  No dynamic reallocation is performed.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

static /* maximum number of terms that can be stored – generous for 10 k */
       const size_t MAX_CAP = 3000000;
static seq_t *seq_arr      = NULL; /* array of sequence values          */
static size_t seq_len      = 0;     /* current number of stored terms */
static idx_t  last_checked = 1;     /* last integer examined           */
static bool   initialized  = false;

/* Initialise the static array once. */
static void ensure_initialised(void)
{
    if (initialized) return;
    seq_arr = mem_new(seq_t, MAX_CAP);
    initialized = true;
}

/* Generate sequence terms until we have at least n terms. */
static void generate_until(idx_t n)
{
    ensure_initialised();
    while (seq_len < (size_t)n) {
        ++last_checked;                     /* next candidate integer   */
        if (!is_prime(last_checked)) continue;
        seq_t cc   = (seq_t)last_checked;
        seq_t diff = cc * cc - (seq_t)2;      /* p^2 - 2                 */
        if (diff % (seq_t)7 != (seq_t)0) {
            seq_arr[seq_len++] = cc;
            continue;
        }
        seq_t q = diff / (seq_t)7;           /* (p^2-2)/7               */
        if (!is_prime_big(q)) {
            seq_arr[seq_len++] = cc;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – ignore n=0    */
    generate_until(n);
    return seq_arr[(size_t)n - 1];
}
