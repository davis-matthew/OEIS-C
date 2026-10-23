/*
 * A126855  Numbers k such that, if k = product{p|k} p^c(k,p), each c(k,p) is
 *          a positive integer and each p is a distinct prime, then the
 *          smallest prime-power p^c(k, p) is not a power of the smallest
 *          prime dividing k.
 * https://oeis.org/A126855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 8189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer k such that
 * in the prime‑power factorisation k = ∏ p^e, the smallest prime power
 * (i.e. the minimal numeric value among the p^e) is not a power of the
 * smallest prime dividing k.  The sequence starts with 12.  a(1) = 12. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *seq = NULL;          /* array storing found terms          */
static size_t seq_len = 0;         /* number of terms stored             */
static size_t seq_cap = 0;         /* capacity of seq                    */

/* The next integer to test.  We start from 2 and skip below 12 later. */
static idx_t candidate = 2;

/* Utility: grow the sequence storage to at least need elements. */
static void grow_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* Test whether n satisfies the sequence condition. */
static bool is_good(idx_t n)
{
    if (n < 2) return false;

    idx_t temp = n;
    idx_t p_min = 0;          /* smallest prime divisor of k       */
    idx_t min_pow = (idx_t)-1;/* minimal prime power value         */
    idx_t min_base = 0;       /* prime base of that power          */

    /* handle factor 2 */
    if (temp % 2 == 0) {
        idx_t power = 1;
        while (temp % 2 == 0) {
            temp /= 2;
            power *= 2;
        }
        p_min = 2;
        min_pow = power;
        min_base = 2;
    }

    /* odd factors up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
            }
            if (p_min == 0) p_min = p;
            if (power < min_pow) {
                min_pow = power;
                min_base = p;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {              /* prime > sqrt(original n)            */
        if (p_min == 0) p_min = temp; /* only one prime divisor              */
        idx_t power = temp;               /* exponent is 1                       */
        if (power < min_pow) {
            min_pow = power;
            min_base = temp;
        }
    }

    /* Condition: smallest prime power is NOT a power of the smallest prime. */
    return min_base != p_min;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.  a(1) is the first term (12). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (candidate < 12) candidate = 12;   /* skip numbers <12   */
        if (is_good(candidate)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq[n - 1];
}
