/*
 * A231175  Let A={2,4,5,8,9,11,14,...} be the sequence of numbers k>=1 such
 *          that k+1 is evil (A001969), and let B be the complement of A. The
 *          sequence lists numbers for which number of A-divisors equals
 *          number of B-divisors.
 * https://oeis.org/A231175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231175: brute‑force search over perfect squares.
   For each square m = s² the proper divisors are enumerated; a divisor
   d is counted in A if popcount(d+1) is even, otherwise in B.
   The square is accepted when |A| == |B|.  The function keeps a static
   list of all found terms so that any call with index n returns the n‑th
   term (1‑based). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Return true iff the binary popcount of x is even. */
static bool even_popcount(seq_t x)
{
    bool parity = false;
    while (x != (seq_t)0) {
        bool bit = ((x & (seq_t)1) != (seq_t)0);
        parity ^= bit;
        x >>= 1;
    }
    return parity == false;
}

/* Recursive divisor counter for m = s². */
static void cnt_rec(size_t idx, seq_t cur, seq_t m,
                    size_t *cntA, size_t *cntB,
                    const uint64_t fac_p[], const int e2[],
                    size_t numf)
{
    if (idx == numf) {
        if (cur == m) return;          /* exclude the number itself */
        seq_t t = cur + (seq_t)1;
        if (even_popcount(t)) ++(*cntA);
        else                 ++(*cntB);
        return;
    }
    const uint64_t p = fac_p[idx];
    seq_t cur2 = cur;
    for (int i = 0; i <= e2[idx]; ++i) {
        cnt_rec(idx + 1, cur2, m, cntA, cntB, fac_p, e2, numf);
        cur2 *= (seq_t)p;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe guard */

    /* static cache of known terms (1‑based) */
    static seq_t *seq_arr   = NULL;
    static size_t seq_cap   = 0;
    static size_t seq_len   = 0;

    /* static prime list (used for factoring s) */
    static uint64_t *primes   = NULL;
    static size_t   primes_cnt = 0;
    static const idx_t PRIME_LIMIT = 2000000;  /* enough for s up to ~4×10¹² */

    /* initialise primes once */
    if (primes == NULL) {
        build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);
    }

    /* static counter for the next s to test */
    static idx_t next_s = 1;

    /* grow cache until we have enough terms */
    while (seq_len < (size_t)n) {
        /* --- candidate square m = s² -------------------------------- */
        idx_t s = next_s++;
        uint64_t fac_p[64];
        int      fac_e[64];
        size_t   numf = 0;
        uint64_t tmp  = (uint64_t)s;

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > tmp) break;
            if (tmp % p == 0) {
                int cnt = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++cnt;
                }
                fac_p[numf] = p;
                fac_e[numf] = cnt;
                ++numf;
            }
        }
        if (tmp > 1) {
            fac_p[numf] = tmp;
            fac_e[numf] = 1;
            ++numf;
        }

        int e2[64];
        for (size_t i = 0; i < numf; ++i) e2[i] = fac_e[i] * 2;

        seq_t m = (seq_t)s * (seq_t)s;

        /* --- count A‑ and B‑divisors ------------------------------ */
        size_t cntA = 0, cntB = 0;
        cnt_rec(0, (seq_t)1, m, &cntA, &cntB, fac_p, e2, numf);

        if (cntA == cntB) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = m;
        }
    }

    return seq_arr[n - 1];
}
