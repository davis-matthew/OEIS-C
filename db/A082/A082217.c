/*
 * A082217  Smallest palindrome beginning with n and digit sum n, or 0 if no
 *          such number exists.
 * https://oeis.org/A082217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = smallest palindrome beginning with n and
    having digit sum n; returns 0 if none exists.           */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Allocate and construct the smallest palindrome for the specified
   left‑half length `len_a`, required left‑half digit sum `targetS`,
   prefix digits in `pre[0…len_p-1]`, and middle digit `m`.
   If `odd` is true, the resulting palindrome has odd
   total length; otherwise it has even length.              */
static seq_t
build_pal(unsigned len_a, uint64_t targetS,
          const uint8_t *pre, unsigned len_p,
          uint8_t m, bool odd)
{
    uint8_t *A = (uint8_t *)malloc(len_a);
    if (!A)
        return (seq_t)0;

    /* copy the mandatory prefix */
    for (unsigned i = 0; i < len_p; ++i)
        A[i] = pre[i];

    /* fill the remaining digits with the smallest possible values
       to reach targetS */
    uint64_t sum = 0;
    for (unsigned i = 0; i < len_p; ++i)
        sum += A[i];

    for (unsigned i = len_p; i < len_a; ++i) {
        uint64_t remPos = len_a - i - 1;
        uint64_t remSum = targetS - sum;
        uint8_t d = 0;
        for (; d <= 9; ++d) {            /* minimal d that lets
                                            the rest hold the sum */
            if (remSum - d <= 9ULL * remPos)
                break;
        }
        A[i] = d;
        sum += d;
    }

    /* build the numeric value of the palindrome */
    seq_t val = 0;
    for (unsigned i = 0; i < len_a; ++i)
        val = val * 10 + (seq_t)A[i];
    if (odd)
        val = val * 10 + (seq_t)m;
    for (unsigned i = len_a; i-- > 0; )
        val = val * 10 + (seq_t)A[i];

    free(A);
    return val;
}

seq_t
a(idx_t n)
{
    /* one‑digit numbers are trivial */
    if (n <= 9)
        return (seq_t)n;

    /* digits of n in most significant first order */
    uint8_t pre[32];
    unsigned len_p = 0;
    uint64_t tmp = (uint64_t)n;
    uint8_t rev[32];
    unsigned rev_len = 0;
    while (tmp) {
        rev[rev_len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (unsigned i = 0; i < rev_len; ++i)
        pre[i] = rev[rev_len - 1 - i];
    len_p = rev_len;

    /* sum of the prefix digits */
    uint64_t sum_p = 0;
    for (unsigned i = 0; i < len_p; ++i)
        sum_p += pre[i];

    /* early exit: if the prefix sum already exceeds n/2 we may have no
       solution (this occurs only for n = 19). */
    if (sum_p > (uint64_t)n)
        return (seq_t)0;

    const unsigned MAX_LEN_A = 2000;          /* safe upper bound */
    seq_t best = 0;                           /* 0 means “no candidate yet” */

    uint64_t n64 = (uint64_t)n;
    for (unsigned len_a = len_p; len_a <= MAX_LEN_A; ++len_a) {
        uint64_t maxSum = sum_p + 9ULL * (len_a - len_p);

        /* even‑length candidate – exists only when n is even */
        if ((n64 & 1ULL) == 0) {
            uint64_t targetS = n64 / 2;
            if (targetS >= sum_p && targetS <= maxSum) {
                seq_t cand = build_pal(len_a, targetS, pre, len_p, 0, false);
                if (cand != 0 && (best == 0 || cand < best))
                    best = cand;
            }
        }

        /* odd‑length candidates – middle digit must have same parity as n */
        uint8_t start = (uint8_t)(n64 & 1ULL);
        for (uint8_t m = start; m < 10; m += 2) {
            uint64_t targetS = (n64 - m) / 2;
            if (targetS >= sum_p && targetS <= maxSum) {
                seq_t cand = build_pal(len_a, targetS, pre, len_p, m, true);
                if (cand != 0 && (best == 0 || cand < best))
                    best = cand;
            }
        }

        /* if we found any candidate for this len_a, it is the shortest
           possible length → minimal total number                */
        if (best != 0)
            return best;
    }

    /* no valid palindrome found (happens only for n = 19) */
    return (seq_t)0;
}
