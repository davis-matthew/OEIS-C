/*
 * A078337  Palindromes which are a nontrivial product of 2 palindromes (which
 *          need not be distinct).
 * https://oeis.org/A078337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, make_palindrome, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078337:
   a(n) is the nth decimal palindrome that can be expressed as a product
   of two other non‑trivial decimal palindromes (both factors > 1).
   This function generates palindromes in increasing order and tests
   each for a suitable factorization. */

/* required headers */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/is_palindrome.h"
#include "../utils/make_palindrome.h"

/* ------------------------------------------------------------ */
/* integer square root for seq_t (avoids overflow for 64‑bit)   */
/* ------------------------------------------------------------ */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x, res = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0) {      /* x == 0 */
            res = 0;
            break;
        }
        if (mid <= x / mid) {   /* mid*mid <= x  (no overflow) */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* ------------------------------------------------------------ */
/* make a palindrome from a prefix                                  */

/* ------------------------------------------------------------ */
/* dynamic array helpers                                         */
/* ------------------------------------------------------------ */
static void ensure_cap_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed > *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ------------------------------------------------------------ */
/* test whether P can be written as a*b with a>1,b>1 and both
   palindrome (decimal).  The palindrome divisors are taken from
   palList (first palSize items).                                 */
/* ------------------------------------------------------------ */
static bool is_product_of_two_palindromes(seq_t P, seq_t *palList, size_t palSize)
{
    seq_t limit = isqrt_seq(P);
    for (size_t i = 0; i < palSize; ++i) {
        seq_t a = palList[i];
        if (a <= 1) continue;
        if (a > limit) break;
        if (P % a == 0) {
            seq_t b = P / a;
            if (b > 1 && is_palindrome(b, 10)) return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------ */
/* main entry point                                               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;

    static seq_t *palList   = NULL;
    static size_t palSize   = 0, palCap   = 0;
    static seq_t *foundVals = NULL;
    static size_t foundSize = 0, foundCap = 0;

    /* generator state for palindromes of current length */
    static idx_t  gen_len = 1;
    static bool   gen_odd = true;
    static seq_t  gen_start = 0, gen_end = 0, gen_cur = 0;

    if (!initialized) {
        initialized = true;
        gen_len   = 1;
        gen_odd   = true;
        gen_start = pow10(0);            /* 10^0 = 1 */
        gen_end   = pow10(1) - 1;        /* 10^1 - 1 = 9 */
        gen_cur   = gen_start;
    }

    while (foundSize < n) {
        /* ---- generate next palindrome ---- */
        seq_t prefix = gen_cur;
        seq_t P = (gen_len == 1) ? prefix : make_palindrome(prefix, gen_odd);

        /* advance generator for next candidate */
        ++gen_cur;
        if (gen_cur > gen_end) {
            ++gen_len;
            size_t half = (gen_len + 1) / 2;
            gen_start = pow10(half - 1);
            gen_end   = pow10(half) - 1;
            gen_cur   = gen_start;
            gen_odd   = (gen_len % 2 == 1);
        }

        /* ---- store this palindrome for future divisor tests ---- */
        ensure_cap_seq(&palList, &palCap, palSize + 1);
        palList[palSize++] = P;

        /* ---- test if it is a product of two palindromes ---- */
        if (is_product_of_two_palindromes(P, palList, palSize)) {
            ensure_cap_seq(&foundVals, &foundCap, foundSize + 1);
            foundVals[foundSize++] = P;
        }
    }

    return foundVals[n - 1];
}
