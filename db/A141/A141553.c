/*
 * A141553  Transformed nonprime products of prime factors of the composites,
 *          the largest prime decremented by 2 and the smallest prime
 *          incremented by 1.
 * https://oeis.org/A141553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For each composite k, let pmin and pmax be its * smallest and
 *            largest prime factors. Replace one occurrence * of pmax by
 *            (pmax-2) and one occurrence of pmin by (pmin+1), * then multiply
 *            all (modified) prime factors. If the product is * prime, the
 *            term is skipped. This function returns the nth * s...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   build_spf, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
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

seq_t a(idx_t n) {
    /* static state for memoisation */
    static idx_t *spf = NULL;                /* smallest prime factor array */
    static idx_t spf_limit = 0;              /* maximum index filled in spf     */
    static idx_t next_k = 4;                 /* next composite to examine       */
    static seq_t *output = NULL;   /* produced sequence values         */
    static size_t out_len = 0, out_cap = 0;   /* current length and capacity     */

    /* initialise the spf array with a modest limit if needed */
    if (spf == NULL) {
        spf_limit = 100;
        build_spf(spf_limit, &spf);          /* allocates turnpike from 0..spf_limit */
    }

    /* generate terms until we have at least n of them */
    while (out_len < n) {
        /* ensure spf covers the next number to examine */
        if (next_k > spf_limit) {
            idx_t new_limit = spf_limit * 2;
            idx_t *new_spf = NULL;
            build_spf(new_limit, &new_spf);
            free(spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        /* skip primes */
        while (next_k <= spf_limit && spf[next_k] == next_k)
            ++next_k;

        if (next_k > spf_limit)   /* safety guard; should never happen */
            continue;

        idx_t k = next_k++;        /* composite to process */

        /* find smallest and largest prime factors */
        idx_t temp = k;
        idx_t pmin = spf[temp];
        idx_t pmax = spf[temp];
        while (temp > 1) {
            idx_t p = spf[temp];
            if (p > pmax) pmax = p;
            while (temp % p == 0) temp /= p;
        }

        /* compute transformed product */
        seq_t prod = 1;
        bool used_pmin = false, used_pmax = false;  /* track one occurrence each */

        temp = k;
        while (temp > 1) {
            idx_t p = spf[temp];
            int mult = 0;
            while (temp % p == 0) {
                ++mult;
                temp /= p;
            }
            for (int i = 0; i < mult; ++i) {
                seq_t val = (seq_t)p;
                if (!used_pmin && p == pmin) {
                    val = (seq_t)(pmin + 1);
                    used_pmin = true;
                } else if (!used_pmax && p == pmax) {
                    val = (seq_t)(pmax - 2);
                    used_pmax = true;
                }
                prod *= val;
            }
        }

        /* skip if product is prime */
        if (!is_prime_big(prod)) {
            /* grow output array if needed */
            if (out_len >= out_cap) {
                size_t new_cap = out_cap ? out_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, output, new_cap);
                if (!tmp) { /* allocation failed – abort */
                    free(spf);
                    mem_free(output);
                    exit(1);
                }
                output = tmp;
                out_cap = new_cap;
            }
            output[out_len++] = prod;
        }
    }

    return output[n - 1];
}
