/*
 * A214285  List of amicable sums-of-factorial-of-digits pairs (A,B): A equals
 *          the sum of the factorials of B's digits in base 10, and vice
 *          versa.
 * https://oeis.org/A214285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A214285: amicable sums‑of‑factorial‑of‑digits pairs.
   The sequence lists numbers A such that A = sum of the factorials of B's
   digits and B = sum of the factorials of A's digits (A ≠ B).  Pairs are
   enumerated by ascending first element, then both elements of the pair. */

#include <stdlib.h>
#include <stdint.h>

/* Pre‑computed factorials of digits 0–9. */
static const seq_t fact[10] = {
    (seq_t)1,    /* 0! */
    (seq_t)1,    /* 1! */
    (seq_t)2,    /* 2! */
    (seq_t)6,    /* 3! */
    (seq_t)24,   /* 4! */
    (seq_t)120,  /* 5! */
    (seq_t)720,  /* 6! */
    (seq_t)5040, /* 7! */
    (seq_t)40320,/* 8! */
    (seq_t)362880/* 9! */
};

/* Sum of factorials of the decimal digits of x (x ≤ 10·9!). */
static inline seq_t sum_fact_digits_idx(idx_t x)
{
    seq_t sum = 0;
    if (x == 0)
        return fact[0];
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        sum += fact[d];
        x /= 10;
    }
    return sum;
}

/* Dynamic array of found pair values. */
static seq_t *pair_vals = NULL;
static size_t pair_cnt   = 0;
static int   initialized = 0;

/* Return the nth entry of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Search up to 10·9! = 3 628 800 – any larger number reduces. */
        const idx_t MAX_N = 3628800ULL;     /* 10·9! */

        /* Estimate upper bound for number of pairs (very conservative). */
        size_t cap = 16;
        pair_vals = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!pair_vals)
            return 0;                       /* allocation failure */

        for (idx_t i = 1; i <= MAX_N; ++i) {
            seq_t m = sum_fact_digits_idx(i);
            if (m == (seq_t)i)                /* ignore fixed points          */
                continue;

            /* m is guaranteed to be ≤ MAX_N, so casting to idx_t is safe. */
            idx_t m_idx = (idx_t)m;
            seq_t m2 = sum_fact_digits_idx(m_idx);
            if (m2 != (seq_t)i)               /* not a mutual pair            */
                continue;

            /* Store each pair once, with the smaller element first. */
            if (i < m_idx) {
                if (pair_cnt + 2 > cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(pair_vals, cap * sizeof(seq_t));
                    if (!tmp) {
                        free(pair_vals);
                        pair_vals = NULL;
                        return 0;           /* allocation failure */
                    }
                    pair_vals = tmp;
                }
                pair_vals[pair_cnt++] = (seq_t)i;
                pair_vals[pair_cnt++] = (seq_t)m;
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > pair_cnt)
        return 0;
    return pair_vals[n - 1];
}
