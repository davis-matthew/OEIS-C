/*
 * A101934  Numbers n with omega(n) smaller than omega(n-1) and omega(n+1).
 * https://oeis.org/A101934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101934: numbers n>1 such that ω(n) < ω(n-1) and ω(n+1) */
/* where ω(n) is the number of distinct prime factors of n.  The function */
/* returns the n-th such number (offset 1).  The implementation keeps a */
/* global cache of computed terms and the ω‑values up to the current max. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; this case is unused */

    /* static state to avoid recomputing */
    static uint16_t *omega = NULL;          /* ω values for each integer */
    static size_t omega_alloc = 0;
    static size_t omega_len  = 0;          /* next index to write */

    static seq_t *terms = NULL;            /* stored sequence terms */
    static size_t terms_alloc = 0;
    static size_t terms_len  = 0;          /* count of terms stored */

    static idx_t next_m = 2;               /* next integer to evaluate ω */

    /* helper to grow ω array */
    if (omega_alloc == 0) {
        omega_alloc = 16;
        omega = (uint16_t *)malloc(omega_alloc * sizeof(uint16_t));
        if (!omega) return (seq_t)0;
        omega[0] = 0;   /* ω(0), unused but safe */
        omega[1] = 0;   /* ω(1) = 0 */
        omega_len  = 2;
    }

    /* helper to grow terms array */
    if (terms_alloc == 0) {
        terms_alloc = 16;
        terms = (seq_t *)malloc(terms_alloc * sizeof(seq_t));
        if (!terms) return (seq_t)0;
    }

    /* compute until we have at least n terms */
    while (terms_len < (size_t)n) {
        /* grow omega if necessary */
        if ((size_t)next_m >= omega_alloc) {
            size_t new_alloc = omega_alloc * 2;
            if (new_alloc <= (size_t)next_m) new_alloc = next_m + 16;
            uint16_t *tmp = (uint16_t *)realloc(omega, new_alloc * sizeof(uint16_t));
            if (!tmp) return (seq_t)0;
            omega = tmp;
            omega_alloc = new_alloc;
        }

        /* compute ω(next_m) */
        uint64_t temp = (uint64_t)next_m;
        uint16_t cnt = 0;

        if ((temp & 1ULL) == 0) {             /* factor 2 */
            cnt++;
            while ((temp & 1ULL) == 0) temp >>= 1;
        }
        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                cnt++;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) cnt++;

        omega[next_m] = cnt;
        if (omega_len <= (size_t)next_m) omega_len = next_m + 1;

        /* candidate number is next_m - 1 */
        if (next_m >= 3) {
            uint16_t w_nm1 = omega[next_m - 1];   /* ω(n)            */
            uint16_t w_nm2 = omega[next_m - 2];   /* ω(n-1)           */
            uint16_t w_n   = omega[next_m];      /* ω(n+1)           */

            if (w_nm1 < w_nm2 && w_nm1 < w_n) {
                /* store term */
                if (terms_len >= terms_alloc) {
                    size_t new_alloc = terms_alloc * 2;
                    if (new_alloc == 0) new_alloc = 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_alloc * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    terms = tmp;
                    terms_alloc = new_alloc;
                }
                terms[terms_len++] = (seq_t)(next_m - 1);
            }
        }

        next_m++;
    }

    /* return cached term */
    return terms[n - 1];
}
