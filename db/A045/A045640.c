/*
 * A045640  Duplicate of A043040.
 * https://oeis.org/A045640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the n-th decimal palindrome that starts and ends with
 *            the digit 5. The sequence consists of all numbers of the form 5,
 *            55, 505, 515, 525, …, i.e. all palindromes whose first digit is
 *            5
 * Range:     i8 n <= 2, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc.  (idx_t and seq_t are provided by the build) */
#include <stdbool.h>  /* if needed for helper APIs */

static const seq_t pow10_arr[20] = {
    1LL,             /* 10^0 */
    10LL,             /* 10^1 */
    100LL,            /* 10^2 */
    1000LL,           /* 10^3 */
    10000LL,          /* 10^4 */
    100000LL,         /* 10^5 */
    1000000LL,        /* 10^6 */
    10000000LL,       /* 10^7 */
    100000000LL,      /* 10^8 */
    1000000000LL,     /* 10^9 */
    10000000000LL,    /* 10^10 */
    100000000000LL,   /* 10^11 */
    1000000000000LL,  /* 10^12 */
    10000000000000LL, /* 10^13 */
    100000000000000LL,/* 10^14 */
    1000000000000000LL,/* 10^15 */
    10000000000000000LL,/* 10^16 */
    100000000000000000ULL,/* 10^17 */
    1000000000000000000ULL,/* 10^18 */
    10000000000000000000ULL/* 10^19 */
};

seq_t a(idx_t n)
{
    /* Find the length L of the palindrome containing a(n). */
    idx_t remaining = n;
    idx_t L = 1;

    while (1) {
        idx_t count;
        if (L == 1 || L == 2) {
            count = 1;
        } else {
            count = (idx_t)pow10_arr[(L - 1) / 2];   /* 10^floor((L-1)/2) */
        }
        if (remaining <= count)
            break;
        remaining -= count;
        ++L;
    }

    /* Offset within this length (0‑based). */
    idx_t offset = remaining - 1;

    /* Direct cases for length 1 and 2. */
    if (L == 1) return (seq_t)5;
    if (L == 2) return (seq_t)55;

    seq_t result = 0;

    if (L & 1) {   /* odd length: 2k+1 */
        idx_t k = (L - 1) / 2;          /* number of free digits after the first 5 */
        /* Extract the k digits of offset in base 10, with leading zeros. */
        signed char B[16] = {0};
        idx_t tmp = offset;
        for (idx_t i = 0; i < k; ++i) {
            B[k - 1 - i] = tmp % 10;
            tmp /= 10;
        }
        result = 5;
        for (idx_t i = 0; i < k; ++i)          /* left part */
            result = result * 10 + B[i];
        for (idx_t i = k - 2; i < k; --i)     /* mirrored part (skip middle digit) */
            result = result * 10 + B[i];
        result = result * 10 + 5;              /* trailing 5 */
    } else {          /* even length: 2k */
        idx_t k = L / 2;                       /* k >= 1 */
        idx_t digits_needed = k - 1;           /* number of free digits after the first 5 */
        signed char B[16] = {0};
        idx_t tmp = offset;
        for (idx_t i = 0; i < digits_needed; ++i) {
            B[digits_needed - 1 - i] = tmp % 10;
            tmp /= 10;
        }
        result = 5;
        for (idx_t i = 0; i < digits_needed; ++i)  /* left part */
            result = result * 10 + B[i];
        for (idx_t i = digits_needed; i > 0; --i)   /* mirrored part */
            result = result * 10 + B[i - 1];
        result = result * 10 + 5;                  /* trailing 5 */
    }
    return result;
}
