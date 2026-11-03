/*
 * A144581  Triangular numbers k*(k+1)/2 such that (number of prime factors of
 *          k) + (number of prime factors of k+1) is composite.
 * https://oeis.org/A144581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number k*(k+1)/2 for which
 *            Ω(k)+Ω(k+1) is composite (Ω = number of prime factors, counted
 *            with multiplicity)
 * Range:     u8 n <= 10, u16 n <= 197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 7776 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/bigomega_seq.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    /* Static state for memoisation of the sequence terms. */
    static idx_t last_k = 0;      /* last tested k (i.e. the last k that was processed) */
    static idx_t found  = 0;      /* number of terms already stored in the cache */
    static seq_t *buf   = NULL;   /* array of accepted triangular numbers */
    static size_t buf_cap = 0;    /* current capacity of |buf| */

    /* Extend the cache until it contains at least n terms. */
    while (found < n) {
        /* Candidate k. */
        idx_t k = ++last_k;

        /* Ω(k) and Ω(k+1). */
        seq_t omega_k  = bigomega_seq((seq_t)k);
        seq_t omega_k1 = bigomega_seq((seq_t)(k + 1));

        /* Sum of the Ω values.  We need it as an idx_t so later we can test
           primality and composite status using the provided helpers. */
        idx_t sum = (idx_t)omega_k + (idx_t)omega_k1;

        /* The sum must be >1 (i.e. not 0 or 1) and not prime.  If it is
           prime or <=1 we skip this k. */
        if (sum <= 1 || is_prime_big((seq_t)sum))
            continue;  /* not valid; try next k */

        /* Compute the triangular number using seq_t arithmetic. */
        seq_t tri = ((seq_t)k) * ((seq_t)k + 1) / 2;

        /* Grow the buffer if necessary (doubling strategy). */
        if (found == buf_cap) {
            size_t new_cap = buf_cap == 0 ? 64 : buf_cap << 1;
            seq_t *new_buf = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < found; ++i)
                new_buf[i] = buf[i];
            if (buf != NULL)
                seq_free_array(buf);
            buf      = new_buf;
            buf_cap  = new_cap;
        }

        /* Store the new triangular number and move to the next slot. */
        buf[found++] = tri;
    }

    /* a(1) corresponds to buf[0] because indices are 1‑based in the OEIS
       definition. */
    return buf[n - 1];
}
