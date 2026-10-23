/*
 * A179872  Numbers h such that antiharmonic mean B(h) of the numbers k < h
 *          such that gcd(k, h) = 1 is not integer.
 * https://oeis.org/A179872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer h (h ≥ 3) such that the antiharmonic *
 *            mean B(h) = (∑_{k<h,gcd(k,h)=1} k²) / (∑_{k<h,gcd(k,h)=1} k) is
 *            not an integer
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;      /* cache of computed sequence values */
static size_t terms_cnt = 0;      /* number of cached terms */
static size_t terms_cap = 0;      /* current capacity of the terms array */

static idx_t next_h = 3;          /* next h to test in the sequence */

static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap == 0 ? 8 : terms_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 */

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < n)
    {
        idx_t h = next_h++;
        /* --- factor h to get its distinct prime factors --- */
        uint64_t pf[12];
        size_t pf_cnt = 0;
        idx_t temp = h;
        for (idx_t p = 2; p * p <= temp; p = (p == 2) ? 3 : p + 2) {
            if (temp % p == 0) {
                pf[pf_cnt++] = (uint64_t)p;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) pf[pf_cnt++] = (uint64_t)temp;

        /* --- compute sums over all 1..h-1 --- */
        unsigned __int128 sum_all1 =
            (unsigned __int128)(h - 1) * h / 2;
        unsigned __int128 sum_all2 =
            (unsigned __int128)(h - 1) * h *
            (2 * (unsigned __int128)h - 1) / 6;

        unsigned __int128 sum_coprime1 = sum_all1;
        unsigned __int128 sum_coprime2 = sum_all2;

        /* --- inclusion–exclusion over subsets of prime factors --- */
        if (pf_cnt > 0) {
            uint64_t max_mask = 1ULL << pf_cnt;
            for (uint64_t mask = 1; mask < max_mask; ++mask) {
                unsigned __int128 d = 1;
                int bits = 0;
                for (size_t i = 0; i < pf_cnt; ++i) {
                    if (mask & (1ULL << i)) {
                        d *= (unsigned __int128)pf[i];
                        ++bits;
                    }
                }
                idx_t D = (idx_t)d;
                idx_t T = (h - 1) / D;

                unsigned __int128 contrib1 =
                    d * (unsigned __int128)T * (T + 1) / 2;
                unsigned __int128 contrib2 =
                    d * d * (unsigned __int128)T *
                    (T + 1) * (2 * (unsigned __int128)T + 1) / 6;

                if (bits & 1) {         /* odd number of primes: subtract */
                    sum_coprime1 -= contrib1;
                    sum_coprime2 -= contrib2;
                } else {                /* even number: add back */
                    sum_coprime1 += contrib1;
                    sum_coprime2 += contrib2;
                }
            }
        }

        /* --- check whether B(h) is an integer --- */
        if (sum_coprime2 % sum_coprime1 != 0) {
            grow_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)h;
        }
    }

    return terms[n - 1];
}
