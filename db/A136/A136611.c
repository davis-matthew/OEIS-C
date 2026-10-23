/*
 * A136611  (0, 1, 2, 3, 2^2, 5, 2*3, 7, 2^3, 3^2, 2*5, 11, 2^2*3, ...)
 *          becomes (0, 1, 2, 3, 4^5, 6, 7*8, 9, 10^11, 12^13, 14*15, 16,
 *          17^18*19, ...).
 * https://oeis.org/A136611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by factorising n (treat 0–3 as the first four
 *            terms), then replacing each atomic number appearing in the
 *            factorisation, from left to right, by the next unused integer,
 *            starting from 4 and counting upward
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sequence element type  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/*  array of prefix sums: prefix[i] = total number of atomic positions for indices < i  */
static idx_t *prefix = NULL;
static size_t  prefix_len = 0;   /* number of valid entries in prefix      */
static size_t  prefix_cap = 0;   /* allocated capacity of prefix            */

/* Initialise the prefix array for the first entry (prefix[0] = 0). */
static void init_prefix(void) {
    if (prefix == NULL) {
        prefix_cap = 8;
        prefix_len = 1;
        prefix = (idx_t *)malloc(prefix_cap * sizeof(idx_t));
        prefix[0] = 0;
    }
}

/* Ensure that prefix[0..n] are computed.  */
static void make_prefix(idx_t n) {
    init_prefix();
    if (prefix_len > n) return;             /* already available */

    /* grow the array if necessary */
    size_t needed = (size_t)n + 1;
    if (prefix_cap < needed) {
        size_t new_cap = prefix_cap > 0 ? prefix_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        prefix = (idx_t *)realloc(prefix, new_cap * sizeof(idx_t));
        prefix_cap = new_cap;
    }

    /* compute missing entries */
    for (size_t i = prefix_len; i <= (size_t)n; ++i) {
        idx_t count = 0;
        idx_t m = (idx_t)(i - 1);         /* count for index i-1   */

        if (m >= 4) {
            idx_t temp = m;
            for (idx_t p = 2; p * p <= temp; ++p) {
                if (temp % p == 0) {
                    size_t exp = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++exp;
                    }
                    count += (exp == 1) ? 1 : 2;   /* base and optional exponent */
                }
            }
            if (temp > 1) {
                /* remaining prime factor with exponent 1 */
                count += 1;
            }
        }

        prefix[i] = prefix[i - 1] + count;
    }

    prefix_len = needed;
}

/* Main sequence function.  */
seq_t a(idx_t n) {
    if (n < 4) return (seq_t)n;          /* trivial first four terms */

    make_prefix(n);                      /* ensure prefix is ready */

    seq_t result   = 1;
    seq_t candidate = (seq_t)4 + prefix[n];  /* first unused integer for a(n) */

    /* factorise n and evaluate the expression.  */
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t base = candidate;
            candidate++;
            seq_t part;
            if (exp == 1) {
                part = base;
            } else {
                seq_t exp_val = candidate;
                candidate++;
                part = big_pow(base, exp_val);
            }
            result = result * part;
        }
    }
    if (temp > 1) {                      /* remaining prime factor */
        seq_t base = candidate;
        candidate++;
        result = result * base;
    }

    return result;
}
