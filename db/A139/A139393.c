/*
 * A139393  a(n) = Sum_{i=1..m} e(i) * 10^(m-i) where e(1) <= ... <= e(m) are
 *          the nonzero exponents in the prime factorization of n: a
 *          representation of the prime signature of n.
 * https://oeis.org/A139393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139393
   Formula: a(n) = ∑_{i=1..m} e_i · 10^{m-i}
   where e_1 ≤ … ≤ e_m are the non‑zero exponents in the prime
   factorisation of n.  For n ≥ 1024 the base 10 is replaced by
   10^⌊1+log10(log2(n))⌋; as n ≤ 2^63−1 this is always 100.
   (That is, the code uses 10 for n < 1024 and 100 otherwise.)   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 is out of OEIS range – return 0 */
    if (n == 0u) return 0;

    /* Special case: 1 has no prime factors → 0   */
    if (n == 1u) return 0;

    /* ---------- factorise n ------------------------------------- */
    uint64_t tmp = n;
    seq_t exps[64];        /* should hold all exponents (≤ 12 in uint64_t) */
    int ecnt = 0;          /* number of exponents recorded */

    /* factor 2 */
    if ((tmp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        exps[ecnt++] = (seq_t)e;
    }

    /* odd factors up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            exps[ecnt++] = (seq_t)e;
        }
    }

    /* remaining prime factor > sqrt */
    if (tmp > 1ULL) {
        exps[ecnt++] = 1;
    }

    /* --------- sort exponents ascending (insertion sort) ---------- */
    for (int i = 1; i < ecnt; ++i) {
        seq_t key = exps[i];
        int j = i - 1;
        while (j >= 0 && exps[j] > key) {
            exps[j + 1] = exps[j];
            --j;
        }
        exps[j + 1] = key;
    }

    /* --------- determine the base (10 or 100) -------------------- */
    seq_t base = (n < 1024u) ? (seq_t)10 : (seq_t)100;

    /* --------- build the result ---------------------------------- */
    seq_t res = 0;
    for (int i = 0; i < ecnt; ++i) {
        res = res * base + exps[i];
    }

    return res;
}
