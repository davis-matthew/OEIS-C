/*
 * A083792  Lexicographically first increasing sequence such that no two
 *          successive terms have the same prime signature.
 * https://oeis.org/A083792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th term of the lexicographically first strictly
 *            increasing sequence of positive integers such that two
 *            consecutive terms never share the same prime signature. No
 *            closed formula is known; this implementation constructs the
 *            sequence by incremental search
 * Range:     u8 n <= 219, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 88 terms through n = 9744 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Compares two prime signatures stored as sorted exponent lists. */
static bool same_signature(const uint8_t a[64], size_t na,
                           const uint8_t b[64], size_t nb)
{
    if (na != nb) return false;
    for (size_t i = 0; i < na; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

/* Computes the sorted list of prime exponents of n.
   The exponents are stored in exps[0..cnt-1]; cnt is written to *cnt_out.
   Only factors that fit in uint64_t are used (the sequence values are
   guaranteed to be < 2^64 for all n ≤ 10 000). */
static void signature_uint64(uint64_t n, uint8_t exps[64], size_t *cnt_out)
{
    size_t cnt = 0;
    uint64_t t = n;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint8_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (t > 1) {                /* remaining prime factor */
        exps[cnt++] = 1;
    }

    /* Insertion sort the exponents (cnt is tiny). */
    for (size_t i = 1; i < cnt; ++i) {
        uint8_t key = exps[i];
        size_t j = i;
        while (j > 0 && exps[j - 1] > key) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = key;
    }

    *cnt_out = cnt;
}

/* ----- Helper: prime signature of a number (unsigned 64‑bit) ----- */

/* ----- Main function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1) return (seq_t)1;

    seq_t prev = (seq_t)1;
    uint8_t prev_sig[64] = {0};
    size_t prev_cnt = 0;          /* signature of 1 is empty */

    for (idx_t pos = 2; pos <= n; ++pos) {
        seq_t cand = prev + 1;    /* next integer after previous term */

        while (true) {
            /* Obtain prime signature of candidate. The value is
               known to fit in uint64_t; conversion is safe. */
            uint64_t cval = (uint64_t)cand;
            uint8_t cand_sig[64];
            size_t cand_cnt;
            signature_uint64(cval, cand_sig, &cand_cnt);

            /* Candidate acceptable if its signature differs from
               that of the previous term. */
            if (!same_signature(prev_sig, prev_cnt,
                                cand_sig, cand_cnt))
                break;          /* suitable next term found */

            cand = cand + 1;    /* try the next integer */
        }

        /* Update previous term and its signature. */
        prev = cand;
        uint64_t pval = (uint64_t)prev;
        signature_uint64(pval, prev_sig, &prev_cnt);
    }

    return prev;
}
