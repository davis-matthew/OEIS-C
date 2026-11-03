/*
 * A065966  Numbers k such that phi(k) / 2 is prime.
 * https://oeis.org/A065966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 759, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that phi(k)/2 is prime: OEIS A065966.
   The nth term a(n) is the nth integer k for which phi(k)/2 is a prime.
   The implementation evaluates phi(k) on demand using the provided
   calc_phi() routine and tests primality of phi(k)/2 with is_prime_big().
   It memoises the sequence values so that arbitrary call order is
   supported.  All operations are performed using seq_t so that the code
   works for the arbitrary‑precision build as well as for fixed width
   integer builds. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
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

/* Static state for the memoised sequence */
static seq_t *seq = NULL;     /* array of found k's */
static size_t seq_len = 0;    /* number of stored elements */
static size_t seq_cap = 0;    /* current allocation size */
static idx_t  next_k  = 1;    /* next integer to test */

/* Ensure the seq array can hold at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap *= 2;
    seq = mem_resize(seq_t, seq, new_cap);
    if (!seq) exit(1);
    seq_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but return 0 */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        /* Compute phi(next_k) */
        seq_t phi_val = totient_seq((seq_t)next_k);

        /* Check if phi is even and phi/2 is prime */
        if ((phi_val & 1) == 0)
        {
            seq_t half = phi_val >> 1;      /* phi/2 */
            if (is_prime_big(half))
            {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = next_k;
            }
        }
        ++next_k;
    }
    return seq[n - 1];
}
