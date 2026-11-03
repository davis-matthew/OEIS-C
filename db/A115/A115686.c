/*
 * A115686  Powerful(1) numbers (A001694) whose digit reversal is a prime
 *          number.
 * https://oeis.org/A115686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115686: Powerful(1) numbers whose decimal reverse is prime
 *
 * The sequence is computed by generating all powerful numbers N
 * of the form a^2 * b^3 up to a sufficiently large limit (1e10),
 * keeping those whose decimal reversal rev(N) is prime.
 * The resulting numbers are sorted, deduplicated, and stored
 * in a static array.  The function returns the (n‑1)‑indexed
 * element of that array.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* cache of the generated sequence */
    static seq_t *seq   = NULL;
    static size_t  seq_len = 0;

    if (seq_len == 0) {
        /* ----------------------------------------------------------
         *  Build the whole sequence once.
         * ---------------------------------------------------------- */
        const idx_t LIMIT = 10000000000ULL;  /* 1·10^10, more than enough for 10 000 terms */

        seq_t *tmp       = NULL;
        size_t len       = 0;
        size_t cap       = 0;

        /* enumerate a²*b³ <= LIMIT */
        for (idx_t a = 1; (idx_t)a * a <= LIMIT; ++a) {
            seq_t a2 = (seq_t)a * (seq_t)a;          /* a²  */
            for (idx_t b = 1;; ++b) {
                seq_t b3 = (seq_t)b * (seq_t)b * (seq_t)b;  /* b³  */
                seq_t val = a2 * b3;
                if (val > (seq_t)LIMIT)
                    break;

                /* reverse in decimal and test primality */
                if (is_prime_big((seq_t)reverse_digits_idx((idx_t)val)))
                    append_candidate(&tmp, &len, &cap, val);
            }
        }

        /* sort the candidates */
        if (len > 1)
            mem_qsort(tmp, len, sizeof(seq_t), compare_seq_t);

        /* deduplicate */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || compare_seq_t(&tmp[uniq - 1], &tmp[i]) != 0)
                tmp[uniq++] = tmp[i];
        }

        seq     = tmp;
        seq_len = uniq;
    }

    /* the sequence is 1‑based in OEIS */
    return seq[n - 1];
}
